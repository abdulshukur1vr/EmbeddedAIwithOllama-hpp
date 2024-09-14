#include "ollama.hpp"
#include "json.hpp"

#include <iostream>
#include <string>

const std::string DEBUG = "false";

const std::string AI = "ZAAS";
const std::string PROMPT = AI + ": ";
const std::string DEFAULT_SERVER_URL = "http://localhost:11434";
const std::string STR_SERVER = "--server";
const std::string STR_PORT = "--port";
const std::string STR_MODEL = "--model";
const std::string STR_EXIT = "exit";
const std::string STR_QUIT = "quit";
const std::string DEFAULT_MODEL = "llama3:8b";

void printusage(const std::string &name) {
	std::cout << "Usage: \n\t" << name << " --server <ip address> --port <port> --model <model>" << std::endl;
}

void intro() {
	std::cout << PROMPT << "Hello, Ask me anything." << std::endl;
	std::cout << PROMPT << "I will try to answer as much as I understand. Please be patient with me." << std::endl;
        std::cout << PROMPT << "Type 'exit' or 'quit' anytime to quit/exit " << AI << " prompt." << std::endl;
        std::cout << PROMPT << "Let's get started..." << std::endl;
}

int main(int argc, char* argv[]) {
	bool debug = false;
	std::string server = "";
	std::string port = "";
	std::string url = DEFAULT_SERVER_URL;
	std::string model = DEFAULT_MODEL;

	if(argc == 1) {
		if(debug == true) std::cout << PROMPT << "Connecting to the server at " << DEFAULT_SERVER_URL << std::endl;
	}
	else {
		if (argv[1] == STR_SERVER) server = argv[2];
		else if(argv[1] == STR_PORT) port = argv[2];
		else if(argv[1] == STR_MODEL) model = argv[2];
		else {
			printusage(argv[0]); return 1;
		}

		if(argv[3] == STR_SERVER) server = argv[4];
		else if(argv[3] == STR_PORT) port = argv[4];
		else if(argv[3] == STR_MODEL) model = argv[4];
		else {
			printusage(argv[0]); return 1;
		}

		if(argv[5] == STR_SERVER) server = argv[6];
		else if(argv[5] == STR_PORT) port = argv[6];
		else if(argv[5] == STR_MODEL) model = argv[6];
		else {
			printusage(argv[0]); return 1;
		}

		if(server.empty() || port.empty() || model.empty()) {
			printusage(argv[0]); return 1;
		}

		url = "http://" + server + ":" + port;
		if(debug == false) std::cout << PROMPT << "Connecting to the server at " << url << std::endl;

	}
	// settign the new url
	// ollama::setServerURL(url);
	Ollama myserver(url);

	// Verify that the server is running
	bool running = myserver.is_running();
	if (running == false) {
		std::cout<< PROMPT << "Server is not running. Please start the server and then start ASAK." << std::endl;
		return 1;
	} 
	
	// introduction
	intro();

	// the version of the server
	// std::string version = ollama::get_version();

	std::string input;
    	while (running) {
        	std::cout << PROMPT;
        	std::getline(std::cin, input);

		std::string response;
		if (input.empty()) response = "Ask me something.";
		else if(myserver.is_running() == false) response = "Server is not running. Please start the server.";
        	else if (input == STR_EXIT || input == STR_QUIT)  {
            		running = false;
            		response = "Thank you for using me. I am Signing off.";
        	} else {
            		std::cout << PROMPT << "Give me a moment..." << std::endl;
			try {
				// if(argc > 1) response = myserver::generate(model, input);
                        	// else response = ollama::generate(model, input);
				response = myserver.generate(model, input);
			}
			catch(ollama::exception e) { 
				// std::cout <<  "Exception: " << e.what() << std::endl;
				response = e.what(); 
			}			
        	}
		// response to the user
                std::cout << PROMPT << response << std::endl;
    	}

	return 0;
}

