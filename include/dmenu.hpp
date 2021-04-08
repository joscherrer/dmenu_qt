#pragma once

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
    QThread workerThread;

    bool event(QEvent *e);
    void filterMenu();
public:
    Dmenu(QWidget *parent = nullptr);
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

