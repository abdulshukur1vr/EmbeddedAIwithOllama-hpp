
#include "utils.hpp"

#include <iostream>
#include <algorithm> // for transform
#include <cctype>    // for tolower
#include <thread>
#include <chrono>

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
    std::cout << "\r" << std::flush;
} // display_spinner

