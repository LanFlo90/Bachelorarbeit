#include "timeInterpolation.hpp"



std::chrono::system_clock::time_point linear_time_interpolation(
    const std::chrono::system_clock::time_point &start,
    const std::chrono::system_clock::time_point &end,
    double t
){
    t = std::max(0.0, std::min(1.0, t));

    auto duration = end - start;
    auto interpolated_duration = std::chrono::duration_cast<std::chrono::milliseconds>(duration * t);

    return start + interpolated_duration;
}

std::string time_interpolation_string_representation(std::chrono::system_clock::time_point p){

    std::ostringstream output;

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch()).count();

    std::time_t interpolated_time = std::chrono::system_clock::to_time_t(p);

    output
            << std::put_time(std::localtime(&interpolated_time), "%Y-%m-%d-%H:%M:%S")
            << "." << std::setw(3)
            << std::setfill('0')
            << (ms % 1000)
            << std::endl;


    return output.str();

}



std::string linear_interpolation_timestamps(const std::string &start_time, const std::string &end_time, double t){

    std::tm start_tm {};
    std::tm end_tm {};

    std::istringstream start_stream(start_time);
    std::istringstream end_stream(end_time);

    start_stream >> std::get_time(&start_tm, "%Y-%m-%d-%H:%M:%S");
    end_stream >> std::get_time(&end_tm, "%Y-%m-%d-%H:%M:%S");

    int milliseconds_start = stoi(start_stream.str().substr(20));
    int milliseconds_end = stoi(end_stream.str().substr(20));

    auto start_point = std::chrono::system_clock::from_time_t(std::mktime(&start_tm)) + std::chrono::milliseconds(milliseconds_start);
    auto end_point = std::chrono::system_clock::from_time_t(std::mktime(&end_tm)) + std::chrono::milliseconds(milliseconds_end);

    auto interpolated_time = linear_time_interpolation(start_point, end_point, t);

    return time_interpolation_string_representation(interpolated_time);

}
