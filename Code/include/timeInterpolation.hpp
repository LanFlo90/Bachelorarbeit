#pragma once 

#include <chrono>
#include <limits>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>



std::chrono::system_clock::time_point linear_time_interpolation(
    const std::chrono::system_clock::time_point &start,
    const std::chrono::system_clock::time_point &end,
    double t
);

std::string time_interpolation_string_representation(std::chrono::system_clock::time_point p);



std::string linear_interpolation_timestamps(const std::string &start_time, const std::string &end_time, double t);
