#include "os.hpp"
#include "dmenu.hpp"
#include "config.h"
#include "stdin.hpp"
#include "helper.hpp"
#include <iostream>
#include <QFile>
#include <QStringList>
#include <QApplication>

int
main(int argc, char* argv[])
{
    int i;
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
    }

    if (generic_isatty)
    {
        exit(0);
    }

    Helper h;
    h.timestamp("Init");
    QApplication app(argc, argv);
    h.timestamp("QApplication");
    Dmenu dmenu(nullptr, sensitive);
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
