#pragma once

#include <QObject>
#include <QStringList>
#include <QThread>
#include <thread>

class StdinReader
{

public:
    QStringList data;
    std::thread sr_thread;

    StdinReader();
    void readStdin();
};
