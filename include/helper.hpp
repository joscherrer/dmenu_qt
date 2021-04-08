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
    time_point<high_resolution_clock> initial_time;
    time_point<high_resolution_clock> last_time;
public:
    Helper();
    std::string time_from_last();
    std::string time_from_start();
    ~Helper() = default;
};

extern Helper h;