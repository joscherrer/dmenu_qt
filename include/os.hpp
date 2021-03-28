#include "unistd.h"

#ifdef _linux_
bool agnostic_isatty = isatty(0);
#endif
#ifdef _WIN64
bool agnostic_isatty = _isatty(0);
#endif