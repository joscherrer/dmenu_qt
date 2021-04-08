#include "unistd.h"
#include <iostream>
#include <QApplication>
#include <QStyleFactory>

#include "dmenu.hpp"
#include "config.hpp"
#include "project.h"
#include "helper.hpp"

#if defined(_linux_)
static bool generic_isatty = isatty(0);
#elif defined(_WIN64)
static bool generic_isatty = _isatty(0);
#endif

Config config;
Helper h;

QPalette
map_to_palette(std::map<QPalette::ColorRole, QColor> palette_map)
{
    QPalette palette;

    for (const auto& [key, value] : palette_map)
    {
        palette.setColor(key, value);
    }
    return palette;
}

static void
usage(void)
{
    fputs("usage: dmenu_qt [-iv] [-fn font_face] [-fs font_size] [-nb bg color] [-nf fg color]\n"
          "                [-sb bg highlight color] [-sf fg highlight color]", stderr);
    exit(1);
}

int
main(int argc, char* argv[])
{
    if (generic_isatty)
        usage();

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v")) {
            std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
            exit(0); 
        } else if (!strcmp(argv[i], "-i")) {
            std::cout << "sensitive" << std::endl;
            config.sensitive = false;
        } else if (!strcmp(argv[i], "-fn"))
            config.font_face = QString(argv[++i]);
        else if (!strcmp(argv[i], "-fs"))
            config.font_size = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-l"))
            config.lines = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-nb")) {
            config.palette_map[QPalette::Base] = QColor(argv[++i]);
            config.palette_map[QPalette::Window] = QColor(argv[++i]);
        } else if (!strcmp(argv[i], "-nf"))
            config.palette_map[QPalette::Text] = QColor(argv[++i]);
        else if (!strcmp(argv[i], "-sb"))
            config.palette_map[QPalette::Highlight] = QColor(argv[++i]);
        else if (!strcmp(argv[i], "-sf"))
            config.palette_map[QPalette::HighlightedText] = QColor(argv[++i]);
        else
            usage();
    }

    config.palette = map_to_palette(config.palette_map);

    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("fusion")); /* We do this so the system palette can be overriden */
    app.setFont(QFont(config.font_face, config.font_size));
    app.setPalette(config.palette);

    Dmenu dmenu;
    dmenu.resize(QSize(600, 300));
    dmenu.setFixedSize(QSize(600,300));
    dmenu.show();
    return app.exec();
}
