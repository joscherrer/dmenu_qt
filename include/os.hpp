#pragma once

#include "unistd.h"

#ifdef _linux_
bool generic_isatty = isatty(0);
#endif
#ifdef _WIN64
bool generic_isatty = _isatty(0);
#endif