#pragma once

#include <QListView>

class MenuView : public QListView
{
private:
    // void paintEvent(QPaintEvent *e);
public:
    MenuView(QWidget *parent);
    void setDirection(Flow flow);
    ~MenuView() = default;
};