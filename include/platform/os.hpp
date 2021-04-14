#pragma once

#include <QtGlobal>
#if defined(Q_OS_WINDOWS)
#include "platform/win32.hpp"
#elif defined(Q_OS_LINUX)
#include "platform/linux.hpp"
#endif