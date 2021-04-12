#pragma once

/*
    Here you can set your default config
    It will be overriden by :
    - $config_dir/config.json
    - The -cf option
    - Other options

    With $config dir being 
    - On linux : $HOME/.config/dmenu_qt
    - On Windows : %APPDATA%/dmenu_qt
*/

#include <QFont>
#include <QColor>
#include <QString>
#include <QPalette>

#include "json.hpp"

using json = nlohmann::json;

struct Config
{
    // Sane defaults that you can change if you recompile
    bool      sensitive    = true;       /* Set filtering's case sensitivity */
    int       border_width = 3;          /* Border color */
    int       lines        = 5;          /* -l option; if nonzero, dmenu uses vertical list with given number of lines */   
    int       font_size    = 14;         /* -fs option */
    QString   font_face    = "Courier";  /* -fn option */

    std::map<QPalette::ColorRole, QColor> palette_map = {
        { QPalette::Text,            QColor("#ffffff") }, /* List fg color */
        { QPalette::Base,            QColor("#484848") }, /* Textbox bg color */
        { QPalette::Window,          QColor("#484848") }, /* List bg color */
        { QPalette::Highlight,       QColor("#edccea") }, /* Highlighted item bg color */
        { QPalette::WindowText,      QColor("#edccea") }, /* This sets the border color ¯\_(ツ)_/¯ */
        { QPalette::HighlightedText, QColor("#484848") }  /* Highlighted item fg color */
    };

    /*
        ⚠ Do not make changes from here ⚠
        Unless you know what you are doing
    */
    void gen_palette() {
        for (const auto& [key, value] : palette_map)
            palette.setColor(key, value);
    }

    void gen_font() {
        font = QFont(font_face, font_size);
    }

    void from_json(json jconf) {
        if (!jconf["lines"].is_null())
            this->lines = jconf["lines"];
        if (!jconf["sensitive"].is_null())
            this->sensitive = jconf["sensitive"];
        if (!jconf["font_size"].is_null())
            this->font_size = jconf["font_size"];
        if (!jconf["font_face"].is_null()) {
            std::string ff = jconf["font_face"];
            this->font_face = QString::fromStdString(ff);
        }
        if (!jconf["border_width"].is_null())
            this->border_width = jconf["border_width"];
        if (!jconf["palette_map"].is_null()) {
            std::map<std::string, std::string> pm = jconf["palette_map"];
            for (auto const& [key, val] : pm)
            {
                QPalette::ColorRole role = enum_index[key];
                palette_map[role] = QColor(val.c_str());
            }
        }
    }

    QFont font;
    QPalette palette;
    std::map<std::string, QPalette::ColorRole> enum_index {
        { "QPalette::Text", QPalette::Text },
        { "QPalette::Base", QPalette::Base },
        { "QPalette::Window", QPalette::Window },
        { "QPalette::Highlight", QPalette::Highlight },
        { "QPalette::WindowText", QPalette::WindowText },
        { "QPalette::HighlightedText", QPalette::HighlightedText },
    };
};

extern Config config;