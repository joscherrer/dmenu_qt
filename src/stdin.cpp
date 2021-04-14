#include "stdin.hpp"
#include "helper.hpp"

#include <iostream>
#include <chrono>


StdinReader::StdinReader()
{
    h.timestamp("Stdin start");
    this->sr_thread = std::thread(&StdinReader::readStdin, this);
}

void
StdinReader::readStdin()
{
    for (std::string line; std::getline(std::cin, line);) {
        this->data += QString::fromStdString(line);
    }
}
