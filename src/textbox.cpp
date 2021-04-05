#include "textbox.hpp"

TextBox::TextBox(QWidget *parent)
:QLineEdit(parent)
{
    this->setFrame(false);
    // QPalette palette;
    // QColor background("#484848");
    // QColor textcolor("#ffffff");
    // palette.setColor(QPalette::Base, background);
    // palette.setColor(QPalette::Text, textcolor);
    // palette.setColor(QPalette::Highlight, background);
    // this->setPalette(palette);
}

void
TextBox::focusOutEvent(QFocusEvent *e)
{
    e->accept();
    this->setFocus();
    this->grabKeyboard();
}