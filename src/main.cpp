#include "ollama.hpp"
#include "json.hpp"
#include "utils.hpp"
#include "logger.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>

const std::string AI = "ZAAS";
const std::string AI_VERSION = "1.0.0"; // initial release

const std::string PROMPT = AI + ": ";
const std::string PROMPT_DEBUG = AI + "_DEBUG: ";

const std::string DEFAULT_SERVER = "localhost";
const std::string DEFAULT_PORT = "11434";
const std::string DEFAULT_SERVER_URL = "http://localhost:11434";
const std::string DEFAULT_MODEL = "llama3:8b";

const std::string STR_EXIT = "exit";
const std::string STR_QUIT = "quit";

void printusage(const std::string &name) {
	std::cout << "Usage: \n\t" << name << " --server <ip address> --port <port> --model <model>" << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "  -h, --help		show this help message" << std::endl;
	std::cout << "  -v, --version   	show program version" << std::endl;
	std::cout << "  -d, --debug true/false  enable debug statements" << std::endl;
	std::cout << "  -l, --logger on/off     turn on/off the logger" << std::endl;
	std::cout << "  -s, --server SERVER     set the server address" << std::endl;
	std::cout << "  -p, --port PORT      	set the port number" << std::endl;
	std::cout << "  -m, --model MODEL     	set the model to use" << std::endl;
	std::cout << "" << std::endl;
}

void printversion(const std::string &name) {
	std::cout << name << "version " << AI_VERSION << std::endl;
}

void intro() {
	std::cout << PROMPT << "Hello, Ask me anything." << std::endl;
	std::cout << PROMPT << "I will try to answer as much as I understand. Please be patient with me." << std::endl;
        std::cout << PROMPT << "Type 'exit' or 'quit' anytime to quit/exit " << AI << " prompt." << std::endl;
        std::cout << PROMPT << "Let's get started..." << std::endl;
}

int main (int argc, char* argv[]) {
	bool debug = false;
	std::string log = "off";
	std::string server = DEFAULT_SERVER;
	std::string port = DEFAULT_PORT;
	std::string url = DEFAULT_SERVER_URL;
	std::string model = DEFAULT_MODEL;

	const std::string command_options = "hvd:s:p:m:";
	static struct option long_options[] = {
	    {"help", 	no_argument,       nullptr, 'h'},	
	    {"version", no_argument,       nullptr, 'v'},	
	    {"debug",   required_argument, nullptr, 'd'},	
	    {"logger",  required_argument, nullptr, 'l'},	
	    {"server",  required_argument, nullptr, 's'},	
	    {"port",    required_argument, nullptr, 'p'},	
	    {"model",   required_argument, nullptr, 'm'},	
	    {0, 	0, 	           0,        0}	
	}; // struct
	
	int opt;
	while ((opt = getopt_long(argc, argv, command_options.c_str(), long_options, nullptr)) != -1) {
		switch (opt) {
			case 'h':
				printusage(argv[0]);
				return 0;

			case 'v':
				printversion(argv[0]);
				return 0;

			case 'd':
				debug = optarg;
				break;

			case 'l':
				log = optarg;
				break;

			case 's':
				server = optarg;
				break;

			case 'p':
				port = optarg;
				break;

			case 'm':
				model = optarg;
				break;
	
			default:
				printusage(argv[0]);
				return 1;
		} // switch
	}// while

	const std::string log_file = AI + get_timestamp() + ".log";
	if(to_lowercase(log) == "on") logger::get_instance().init(log_file);

	if ( (!server.empty()) || (!port.empty()) ) url = "http://" + server + ":" + port;
	if (debug == true) {
		if(!url.empty()) std::cout << PROMPT_DEBUG << "Connecting to the server at " << url << std::endl; 
		else std::cout << PROMPT_DEBUG << "Connecting to the server at " << DEFAULT_SERVER_URL << std::endl;
	}

	// setting the url
	Ollama myserver(url);

	if(debug == true) {
		// enabling the debug statements.
		ollama::show_requests(true);
		ollama::show_replies(true);
	}

	// Verify that the server is running
	bool running = myserver.is_running();
	if (running == false) {
		std::cout << PROMPT << "Server is not running. Please start the server and then start " << AI << std::endl;
		return 1;
	}
	else {
		std::cout << PROMPT << "Success. Connected to the server successfully." << std::endl;
	}
	
	// the version of the server
	std::string version = myserver.get_version();
	if (debug == true) std::cout << PROMPT_DEBUG << "server version: " << version << std::endl;
	
	// introduction
	intro();

	std::string input;
	std::string response;
	std::string timespent;
    	while (running) {
		if(to_lowercase(log) == "on") logger::get_instance().log(get_timestamp() + " ");
		std::cout << PROMPT;
        	std::getline(std::cin, input);
		if(to_lowercase(log) == "on") logger::get_instance().log(input+"\n");
		
		if (input.empty()) response = "Ask me something.";
        	else if ( (input.length() == STR_EXIT.length()) && (to_lowercase(input) == STR_EXIT || to_lowercase(input) == STR_QUIT) )  {
            		running = false;
            		response = "Thank you for using me. I am Signing off.";
        	}
		else if (myserver.is_running() == false) response = "Server is not running. Please start the server.";
		else {
            		// std::cout << PROMPT << "Give me a moment..." << std::endl;
			
			bool stop = false;
			std::thread progress(display_spinner, std::ref(stop));
			try {
				auto start_time = get_time_now();
				response = myserver.generate(model, input);
				auto end_time = get_time_now();
				timespent = get_time_diff(start_time, end_time);
			}
			catch (ollama::exception e) { 
				if (debug == true) std::cout << PROMPT_DEBUG << "Exception occured. The message is the following." << std::endl;
				response = e.what();
			}
			stop = true;
			progress.join();
			std::cout<< std::endl;
        	}
		// response to the user
		logger::get_instance().log(get_timestamp() + " " + timespent + " ");
                std::cout << PROMPT << response << std::endl;
		timespent = "";
    	} // while

	if(to_lowercase(log) == "on") logger::get_instance().restore();

	return 0;
} // main

