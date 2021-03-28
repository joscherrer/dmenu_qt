#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QLineEdit>
#include <QListWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>

class Tester : public QMainWindow
{
    Q_OBJECT

    int32_t mButtonPressedAmt;

    QLabel* mpLabel;
    QPushButton* mpPushButton;

    void updateLabelText();

public:
    Tester(QWidget* parent = nullptr);
    ~Tester() = default;

signals:
    void sigLabelTextUpdated(std::string_view);
};

class Dmenu : public QFrame
{
    Q_OBJECT

    QLineEdit* textbox;
    QListWidget* menu;
    bool event(QEvent *e);
    void filterMenu();
public:
    Dmenu(QWidget *parent = nullptr);
    void setMenu(QStringList data);
    void oldSetMenu(QStringList data);
    void printMenu(QStringList data);
    ~Dmenu() = default;

private:
    QListView *menuView;
    QStandardItemModel *menuModel;
    QSortFilterProxyModel *menuProxyModel;

private slots:
    void slotFilterMenu();
};
