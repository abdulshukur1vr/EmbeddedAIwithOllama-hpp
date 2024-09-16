// utils.hpp

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <chrono>

void display_spinner(bool& stop); 
std::string to_lowercase(const std::string& input);
std::string get_timestamp();
std::chrono::system_clock::time_point get_time_now();
std::string get_time_diff(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end);

#endif // __UTILS_HPP__
