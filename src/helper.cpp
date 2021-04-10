#include "helper.hpp"
#include <iomanip>


Helper::Helper()
{
    this->initial_time = high_resolution_clock::now();
    this->last_time = high_resolution_clock::now();
}

double
Helper::time_from_last()
{
    auto tmp_time = high_resolution_clock::now();
    duration<double, std::milli> ms_double = tmp_time - this->last_time;
    this->last_time = tmp_time;
    return ms_double.count();
}

double
Helper::time_from_start()
{
    auto tmp_time = high_resolution_clock::now();
    duration<double, std::milli> ms_double = tmp_time - this->initial_time;
    return ms_double.count();
}

void
Helper::timestamp(std::string msg)
{
    if (!header)
    {
        std::cout << std::left << std::setw(msg_width) << std::setfill(separator) << "Message";
        std::cout << std::left << std::setw(tms_width) << std::setfill(separator) << "Last";
        std::cout << std::left << std::setw(tms_width) << std::setfill(separator) << "Cumul." << std::endl;
        this->header = true;
    }
    double from_start, from_last;
    from_start = this->time_from_start();
    from_last = this->time_from_last();
    std::cout << std::left << std::setw(msg_width) << std::setfill(separator) << msg;
    std::cout << std::fixed << std::setprecision(2) << std::left << std::setw(tms_width) << std::setfill(separator) << from_last;
    std::cout << std::fixed << std::setprecision(2) << std::left << std::setw(tms_width) << std::setfill(separator) << from_start << std::endl;
}