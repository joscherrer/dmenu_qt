#pragma once

/*
    Here you can set your default config
    It will be overriden by any command line option
*/

#include <QPalette>
#include <QColor>
#include <QString>

struct Config
{
    bool      sensitive    = true; /* Set filtering's case sensitivity */
    int       border_width = 3;    /* -l option; if nonzero, dmenu uses vertical list with given number of lines */
    int       lines        = 5;
    int       font_size    = 14;
    QString   font_face    = "Courier";

    std::map<QPalette::ColorRole, QColor> palette_map = {
        { QPalette::Text,            QColor("#ffffff") }, /* List fg color */
        { QPalette::Base,            QColor("#484848") }, /* Textbox bg color */
        { QPalette::Window,          QColor("#484848") }, /* List bg color */
        { QPalette::Highlight,       QColor("#edccea") }, /* Highlighted item bg color */
        { QPalette::WindowText,      QColor("#edccea") }, /* This sets the border color ¯\_(ツ)_/¯ */
        { QPalette::HighlightedText, QColor("#484848") }  /* Highlighted item fg color */
    };

    QPalette palette;
};

extern Config config;