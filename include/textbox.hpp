#pragma once

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>

class TextBox : public QLineEdit
{
    Q_OBJECT

public:
    TextBox(QWidget *parent = nullptr);
    void focusOutEvent(QFocusEvent *e) override;
    ~TextBox() = default;
};
