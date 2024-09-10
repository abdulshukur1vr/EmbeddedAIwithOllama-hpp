#include "ollama.hpp"
#include "json.hpp"

#include <iostream>
#include <string>

constexpr const char PROMPT_ASAK[] = "ASAK: ";

void asak_intro() {
	std::cout << PROMPT_ASAK << "Hello, I am ASAK. Ask me anything." << std::endl;
	std::cout << PROMPT_ASAK << "I will try to answer as much as I can ..." << std::endl;
        std::cout << PROMPT_ASAK << "Type 'exit' or 'quit' anytime to quit/exit ASAK prompt." << std::endl;
}

int main() {

	// Verify that the Ollama server is running with ollama::is_running()
	bool running = ollama::is_running();

	if (running == false) {
		std::cout<<"INFO: Ollama Server is not running. \n\tPlease start the Ollama server and then start ASAK." << std::endl;
		return 1;
	} 

	// the version of the ollama server using ollama::get_version()
	std::string version = ollama::get_version();
	std::cout << "(Ollama Version = " << version << ")" << std::endl;

	asak_intro();

	std::string input;
    	while (running) {
        	std::cout << PROMPT_ASAK;
        	std::getline(std::cin, input);

        	if (input == "exit" || input == "quit")  {
            		running = false;
            		std::cout << PROMPT_ASAK << "Thank you for using me. I am Signing off." << std::endl;
        	} else {
            		std::cout << PROMPT_ASAK << "ok. Give me a moment..." << std::endl;
	                try {
                        	std::string output = ollama::generate("llama3:8b", input);
                        	std::cout << "ASAK: " << output << std::endl;
			}
			catch(ollama::exception e) { std::cout <<  "Exception: " << e.what() << std::endl; }			
        	}
    	}

	return 0;
}
