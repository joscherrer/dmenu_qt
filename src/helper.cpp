#include "helper.hpp"

Helper::Helper()
{
    this->last_time = high_resolution_clock::now();
}


void Helper::timestamp(std::string s)
{
    auto tmp_time = high_resolution_clock::now();
    duration<double, std::milli> ms_double = tmp_time - this->last_time;
    this->last_time = tmp_time;
    std::cout << s << ": " << ms_double.count() << "ms" << std::endl;
}