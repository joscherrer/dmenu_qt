#pragma once

#include <string>
#include <iostream>
#include <QPalette>
#include <QColor>
#include <QString>

const QString dmenu_font = "Courier";

// normal bg   : #484848 
// normal fg   : white
// selected bg : #edccea
// selected fg : #484848

// I don't know why I did this, really
using namespace std::string_literals;
const std::string dmenu_style = R"(
*
{
    background-color: %s;
    color: %s;
    font-size: 30px;
    margin: 0px;
    padding: 0px;
}

.Dmenu
{
    border: 3px solid #edccea;
}

QLineEdit
{
    border: none;
    padding: 0;
    selection-background-color: #edccea;
    selection-color: black;
}

QScrollBar {
    width:0px;
}

QListView
{
    padding: 0;
    margin: 0;
}

QListView::item
{
    padding: 0px;
    margin: 0px;
    height: 45px;
    font-size: 30px;
}

QListView::item::selected
{
    background-color: %s;
    color: %s;
}

QListWidget::item:hover,
QListWidget::item:disabled:hover,
QListWidget::item:hover:!active,
{background: transparent;}
)"s;

