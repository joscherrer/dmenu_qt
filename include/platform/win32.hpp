#pragma once

#include <QPoint>
#include <QStandardPaths>

auto conf_base = QStandardPaths::AppDataLocation;

QPoint os_active_window_pos();
bool os_isatty();