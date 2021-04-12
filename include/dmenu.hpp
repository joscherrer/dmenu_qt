#pragma once

#include "stdin.hpp"

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

    QLineEdit* textBox;

    bool event(QEvent *e);
    void filterMenu();
public:
    StdinReader* sr;
    Dmenu(QWidget *parent = nullptr);
    void focusOutEvent(QFocusEvent *e) override;
    QModelIndex selectRow(int row);
    void fitToContent();
    ~Dmenu() = default;

private:
    QListView *menuView;
    QStringListModel *menuModel;
    QSortFilterProxyModel *menuProxyModel;

public slots:
    void addEntry(QStringList &entry);
    void set_data();

signals:
    void readStdin();
};

