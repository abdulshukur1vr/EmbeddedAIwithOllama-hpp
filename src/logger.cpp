#include "logger.hpp"
#include <cstdlib>

// init static member
logger* logger::instance = nullptr;

logger& logger::get_instance() {
	if(!instance) {
		instance = new logger();
	}
	return *instance;
}

logger::logger()
	: _cin_buff(std::cin.rdbuf()),
	  _cout_buff(std::cout.rdbuf()),
	  _cerr_buff(std::cerr.rdbuf()),
	  _log_buff(nullptr)
{}

logger::~logger() {}


void logger::init(const std::string& log_file) {

	logfile.open(log_file);
	if(!logfile.is_open()) { std::cerr << "Error opening logfile." << log_file << std::endl; std::exit(1); }
	
	// save the logfile buffer
	_log_buff = logfile.rdbuf();

	// re-direct to logger
	// std::cin.rdbuf(this);
	std::cout.rdbuf(this);
	std::cerr.rdbuf(this);
}

void logger::restore() {
	// restore the buffers
	if(_cin_buff) std::cin.rdbuf(_cin_buff);
	if(_cout_buff) std::cout.rdbuf(_cout_buff);
	if(_cerr_buff) std::cerr.rdbuf(_cerr_buff);

	logfile.close();
	_log_buff = nullptr;
}

int logger::overflow(int ch) {
	if(ch != EOF) {
		char c = static_cast<char>(ch);
		// enable for debug only.
		// if(_cout_buff) _cout_buff->sputc(c);
		// if(_cin_buff) _cin_buff->sputc(c);
		if(_log_buff) _log_buff->sputc(c);
		if(_cerr_buff) _cerr_buff->sputc(c);	
	}
	return 0;
}

void logger::log(const std::string& log_message) {
	if(_log_buff) {
		for(char c : log_message) {
			_log_buff->sputc(c);
		}
	}
}
