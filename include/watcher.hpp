#include <QObject>
#include "stdin.hpp"

class Watcher : public QObject
{
    Q_OBJECT
private:
    StdinReader *stdin_reader;
public:
    Watcher(StdinReader *sr);
    ~Watcher() = default;

public slots:
    void watch_sr();
signals:
    void data_ready();
};
