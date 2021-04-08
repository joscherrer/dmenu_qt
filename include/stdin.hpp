#pragma once

#include <QObject>

class StdinReader : public QObject
{
    Q_OBJECT
public:
    StdinReader();

public slots:
    void readStdin();
signals:
    void newEntry(QStringList entry);
    void stdinDone();
};
