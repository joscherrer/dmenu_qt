#pragma once

#include <QPoint>
#include <QStandardPaths>

auto conf_base = QStandardPaths::ConfigLocation;

QPoint os_active_window_pos();
bool os_isatty();