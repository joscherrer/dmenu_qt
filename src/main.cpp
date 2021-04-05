#include "os.hpp"
#include "dmenu.hpp"
#include "config.hpp"
#include "project.h"
#include "stdin.hpp"
#include "helper.hpp"
#include <iostream>
#include <QFile>
#include <QStringList>
#include <QApplication>
#include <memory>
#include <stdexcept>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

int
main(int argc, char* argv[])
{
    int i;
    QFont font(dmenu_font, 12);
    bool sensitive = true;
    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v"))
        {
            std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
            exit(0);
        } 
        else if (!strcmp(argv[i], "-i"))
        {
            sensitive = false;
        }
        else if (!strcmp(argv[i], "-fn"))
        {
            font = QFont(argv[++i]);
        }
    }

    if (generic_isatty)
    {
        exit(0);
    }

    
    std::string dmenu_formatted = string_format(dmenu_style, "#484848", "white", "#edccea", "#484848");
    std::cout << dmenu_formatted << std::endl;

    Helper h;
    h.timestamp("Init");
    QApplication app(argc, argv);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Normal, QPalette::Highlight, QColor("black"));
    pal.setColor(QPalette::Inactive, QPalette::Highlight, QColor("black"));
    pal.setColor(QPalette::Normal, QPalette::HighlightedText, QColor("black"));
    pal.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor("black"));
    app.setPalette(pal);

    font.setFixedPitch(true);
    font.setStyleHint(QFont::Monospace);
    app.setFont(font);
    h.timestamp("QApplication");
    Dmenu dmenu(sensitive);
    // dmenu.setStyleSheet(QString::fromStdString(dmenu_formatted));
    h.timestamp("Dmenu");
    StdinReader t;

    dmenu.resize(QSize(600, 300));
    dmenu.setFixedSize(QSize(600,300));
    dmenu.show();
    h.timestamp("Show");
    dmenu.startThread();
    h.timestamp("StartThread");
    dmenu.selectRow(0);
    return app.exec();
}
