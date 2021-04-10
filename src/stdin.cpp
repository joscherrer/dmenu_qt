#include "stdin.hpp"
#include <iostream>
#include <chrono>

#include "helper.hpp"

StdinReader::StdinReader()
{
    h.timestamp("Stdin start");
    this->sr_thread = std::thread(&StdinReader::readStdin, this);
}

void
StdinReader::readStdin()
{
    for (std::string line; std::getline(std::cin, line);) {
        data += QString::fromStdString(line);
    }
}
