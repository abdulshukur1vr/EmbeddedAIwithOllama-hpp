
#include "utils.hpp"

#include <iostream>
#include <algorithm> // for transform
#include <cctype>    // for tolower
#include <thread>
#include <chrono>

#include <ctime>
#include <iomanip>
#include <sstream>

std::string to_lowercase(const std::string& mystring) {
    std::string output = mystring;
    std::transform(output.begin(), output.end(), output.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return output;  // Return the lowercase string
} // to_lowercase

void display_spinner(bool& stop) {
    const std::string spinner[] = {"/", "-", "\\", "|"};
    int index = 0;
    while (!stop) {
	std::cout << "\r" << spinner[index] << "" << std::flush;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	index = ( (index + 1) % 4);
    } // while
    std::cout << "\r " << std::flush;
    std::cout << "\r" << std::flush;
} // display_spinner

std::string get_timestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return oss.str();
}

std::chrono::system_clock::time_point get_time_now() {
	return std::chrono::system_clock::now();
}

std::string get_time_diff(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end) {
    auto duration = end - start;
	
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    auto msecs = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 1000;
	
    std::ostringstream oss;
    oss << secs << "s" << msecs << "ms";

    return oss.str();
}

