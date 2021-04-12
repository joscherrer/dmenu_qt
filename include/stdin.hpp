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

    StdinReader();
    void readStdin();
};
