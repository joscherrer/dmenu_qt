#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QListView>
#include <QThread>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <chrono>

class Dmenu : public QFrame
{
    Q_OBJECT

    QLineEdit* textBox;
    QThread workerThread;

    bool event(QEvent *e);
    void filterMenu();
public:
    Dmenu(QWidget *parent = nullptr, bool sensitive = true);
    void focusOutEvent(QFocusEvent *e) override;
    void selectRow(int row);
    void startThread();
    ~Dmenu();

private:
    QListView *menuView;
    QStringListModel *menuModel;
    QSortFilterProxyModel *menuProxyModel;

public slots:
    void addEntry(QStringList entry);
    void slotStartThread();

signals:
    void readStdin();

};
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::time_point;

void get_time(time_point<high_resolution_clock> t1, time_point<high_resolution_clock> t2, QString s);
