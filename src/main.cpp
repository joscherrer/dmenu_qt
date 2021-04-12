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
static bool generic_isatty = isatty(0);
QDir config_dir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + PROJECT_NAME;
#elif defined(_WIN64)
QDir config_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + PROJECT_NAME;
static bool generic_isatty = _isatty(0);
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
    StdinReader* sr = new StdinReader();
    QApplication* app;
    Dmenu* dmenu;

    if (generic_isatty)
        usage();

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
    // exit(0);
    // config.palette = map_to_palette(config.palette_map);
    config.gen_palette();
    config.gen_font();
    h.timestamp("QApp before");
    app = new QApplication(argc, argv);
    app->setStyle(QStyleFactory::create("fusion")); /* We do this so the system palette can be overriden */
    app->setFont(config.font);
    app->setPalette(config.palette);
    h.timestamp("QApp after");


    dmenu = new Dmenu();
    dmenu->sr = sr;
    // Ugly hack to check if stdin data has been loaded completely
    QThread q;
    Watcher *w = new Watcher(sr);
    w->moveToThread(&q);
    w->connect(&q, &QThread::finished, w, &QObject::deleteLater);
    w->connect(dmenu, &Dmenu::readStdin, w, &Watcher::watch_sr);
    w->connect(w, &Watcher::data_ready, dmenu, &Dmenu::set_data);
    q.start();
    dmenu->readStdin();
    // dmenu->setFixedSize(QSize(600,300));
    dmenu->show();
    // dmenu->resize(QSize(600, 300));
    dmenu->fitToContent();
    int rc = app->exec();
    q.quit();
    q.wait();
    return rc;
}
