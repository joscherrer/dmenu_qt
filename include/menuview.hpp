#pragma once

#include <QListView>

class MenuView : public QListView
{
public:
    MenuView(QWidget *parent);
    void setDirection(Flow flow);
    ~MenuView() = default;
};