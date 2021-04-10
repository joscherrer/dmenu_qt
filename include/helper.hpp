#pragma once

#include <chrono>
#include <iostream>
#include <string>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::time_point;

class Helper
{
private:
    const char separator = ' ';
    const int msg_width = 25;
    const int tms_width = 15;
    bool header = false;
    time_point<high_resolution_clock> initial_time;
    time_point<high_resolution_clock> last_time;
public:
    Helper();
    double time_from_last();
    double time_from_start();
    void timestamp(std::string msg);
    ~Helper() = default;
};

extern Helper h;