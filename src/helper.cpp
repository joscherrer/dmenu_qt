#include "helper.hpp"

Helper::Helper()
{
    this->initial_time = high_resolution_clock::now();
    this->last_time = high_resolution_clock::now();
}

std::string Helper::time_from_last()
{
    auto tmp_time = high_resolution_clock::now();
    duration<double, std::milli> ms_double = tmp_time - this->last_time;
    this->last_time = tmp_time;
    return std::to_string(ms_double.count());
}

std::string Helper::time_from_start()
{
    auto tmp_time = high_resolution_clock::now();
    duration<double, std::milli> ms_double = tmp_time - this->initial_time;
    return std::to_string(ms_double.count());
}

