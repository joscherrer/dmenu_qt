#include "watcher.hpp"
#include "helper.hpp"

Watcher::Watcher(StdinReader *sr)
{
    this->sr = sr;
}

void Watcher::watchStdinReader()
{
    this->sr->sr_thread.join();
    h.timestamp("Stdin end");
    emit this->dataReady();
}