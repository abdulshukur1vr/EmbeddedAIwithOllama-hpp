#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <streambuf>
#include <fstream>
#include <iostream>
#include <stdexcept>

class logger : public std::streambuf {
	private:
		logger();
		~logger();
		// prevent copying
		logger(const logger&) = delete;
		logger& operator=(const logger&) = delete;

		static logger* instance;
		std::streambuf* _cin_buff;
		std::streambuf* _cout_buff;
		std::streambuf* _cerr_buff;
		std::streambuf* _log_buff;
		std::ofstream logfile;
	public:
		static logger& get_instance();
		void init(const std::string& log_file);
		void restore();
		void log(const std::string& log_message);
	protected:
		int overflow(int ch) override;
}; // class logger

#endif // __LOGGER_HPP__
