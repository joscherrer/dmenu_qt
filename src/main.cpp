#include "os.hpp"
#include "window.hpp"
#include <QApplication>
#include <iostream>
#include <string>
#include <QStringList>

int main(int argc, char* argv[])
{
    QStringList piped_input;

    if (agnostic_isatty)
    {
        exit(0);
    }

    for (std::string line; std::getline(std::cin, line);) {
        piped_input += QString::fromStdString(line);
    }

    QApplication app(argc, argv);
    Dmenu dmenu;

    dmenu.oldSetMenu(piped_input);
    dmenu.setMenu(piped_input);
    dmenu.resize(QSize(600, 300));
    dmenu.show();
    return app.exec();
}