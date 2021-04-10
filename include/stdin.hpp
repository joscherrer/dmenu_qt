#pragma once

#include <QObject>
#include <QStringList>
#include <thread>

class StdinReader
{

public:
    QStringList data;
    QObject *watcher;
    std::thread sr_thread;
    // bool data_ready = false;
    StdinReader();
    void readStdin();
};
