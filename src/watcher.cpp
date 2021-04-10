#include "watcher.hpp"
#include "helper.hpp"

Watcher::Watcher(StdinReader *sr)
{
    this->stdin_reader = sr;
}

void Watcher::watch_sr()
{
    this->stdin_reader->sr_thread.join();
    h.timestamp("Stdin end");
    emit this->data_ready();
}