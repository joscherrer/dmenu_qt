#include "platform/os.hpp"
#include "unistd.h"
#include <string>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDir>
#include <QStandardPaths>
#include <QApplication>
#include <QStyleFactory>

#include "dmenu.hpp"
#include "config.hpp"
#include "project.h"
#include "helper.hpp"
#include "json.hpp"
#include "stdin.hpp"
#include "watcher.hpp"

#if defined(unix)
auto conf_base = QStandardPaths::ConfigLocation
#elif defined(_WIN64)
auto conf_base = QStandardPaths::AppDataLocation;
#endif

using json = nlohmann::json;

Config config;
Helper h;

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
    // RECT r;
    // char wnd_title[256];
    // HWND hw = GetForegroundWindow();
    // GetWindowTextA(hw, wnd_title, sizeof(wnd_title));
    // std::cout << wnd_title << std::endl;
    // GetWindowRect(hw, &r);
    // std::cout << "x: " << r.left << std::endl;
    // std::cout << "y: " << r.top << std::endl;
    // exit(0);
    if (os_isatty())
        usage();

    StdinReader sr;

    QDir config_dir = QDir(QStandardPaths::writableLocation(conf_base) + "/" + PROJECT_NAME);
    QFile config_file(config_dir.path() + "/config.json");

    if (config_file.exists()) {
        std::ifstream user_conf(config_file.fileName().toStdString());
        json user_json;
        user_conf >> user_json;
        config.from_json(user_json);
    }
    
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-cf")) {
            std::ifstream extra_conf(argv[++i]);
            json extra_json;
            extra_conf >> extra_json;
            config.from_json(extra_json);
        }
    }

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v")) {
            std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
            exit(0); 
        } else if (!strcmp(argv[i], "-i")) {
            config.sensitive = false;
        } else if (!strcmp(argv[i], "-fn"))
            config.font_face = QString(argv[++i]);
        else if (!strcmp(argv[i], "-fs"))
            config.font_size = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-l"))
            config.lines = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-nb")) {
            int val = ++i;
            config.palette_map[QPalette::Base] = QColor(argv[val]);
            config.palette_map[QPalette::Window] = QColor(argv[val]);
        } else if (!strcmp(argv[i], "-nf"))
            config.palette_map[QPalette::Text] = QColor(argv[++i]);
        else if (!strcmp(argv[i], "-sb"))
            config.palette_map[QPalette::Highlight] = QColor(argv[++i]);
        else if (!strcmp(argv[i], "-sf"))
            config.palette_map[QPalette::HighlightedText] = QColor(argv[++i]);
        else if (!strcmp(argv[i], "-cf"))
            ++i; // Ignore config file as its settings are already loaded
        else
            usage();
    }
    h.timestamp("Config start");
    config.gen_palette();
    config.gen_font();
    h.timestamp("Config end");
    h.timestamp("QApp start");
    QApplication app(argc, argv);
    /* We do this so the system palette can be overriden */
    app.setStyle(QStyleFactory::create("fusion"));
    app.setPalette(config.palette);
    app.setFont(config.font);
    h.timestamp("QApp end");

    Dmenu dmenu(&sr);
    emit dmenu.getStdin();
    dmenu.show();
    h.timestamp("dmenu.show()");
    dmenu.fitToContent();
    h.timestamp("dmenu.fitToContent()");
    return app.exec();
}
