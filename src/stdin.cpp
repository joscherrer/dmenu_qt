#include "stdin.hpp"
#include <iostream>
#include <chrono>
#include <thread>

StdinReader::StdinReader()
{
}


void StdinReader::readStdin()
{
    int i = 0;
    QStringList input_data;
    for (std::string line; std::getline(std::cin, line);) {
        // emit newEntry(QString::fromStdString(line));
        i++;
        input_data += QString::fromStdString(line);
        // if (i % 2000 == 0)
        // {
        //     emit newEntry(input_data);
        //     input_data.clear();
        //     // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // }
    }
    emit newEntry(input_data);
}