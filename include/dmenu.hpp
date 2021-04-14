#pragma once

#include "textbox.hpp"
#include "stdin.hpp"
#include "watcher.hpp"

#include <QFrame>
#include <QEvent>
#include <QLineEdit>
#include <QKeyEvent>
#include <QThread>
#include <QListView>
#include <QStringListModel>
#include <QSortFilterProxyModel>

class Dmenu : public QFrame
{
    Q_OBJECT

public:
    Dmenu(StdinReader *sreader, QWidget *parent = nullptr);
    void focusOutEvent(QFocusEvent *e) override;
    void fitToContent();
    QModelIndex selectRow(int row);
    ~Dmenu();

private:
    TextBox *textBox;
    QListView *menuView;
    QStringListModel *menuModel;
    QSortFilterProxyModel *menuProxyModel;
    QThread *q;
    Watcher *w;
    StdinReader *sr;
    bool event(QEvent *e);
    void connectAll();

public slots:
    void addEntry(QStringList &entry);
    void setData();

signals:
    void getStdin();
};

