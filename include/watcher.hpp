#pragma once

#include <QObject>
#include "stdin.hpp"

class Watcher : public QObject
{
    Q_OBJECT
private:
    StdinReader *sr;
public:
    Watcher(StdinReader *sr);
    ~Watcher() = default;

public slots:
    void watchStdinReader();
signals:
    void dataReady();
};
