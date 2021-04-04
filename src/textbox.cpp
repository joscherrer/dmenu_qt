#include "textbox.hpp"

TextBox::TextBox(QWidget *parent)
:QLineEdit(parent)
{
}

void
TextBox::focusOutEvent(QFocusEvent *e)
{
    e->accept();
    this->setFocus();
    this->grabKeyboard();
}