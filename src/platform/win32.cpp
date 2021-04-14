#include "platform/win32.hpp"
#include "unistd.h"
#include "windows.h"

QPoint os_active_window_pos()
{

}

bool os_isatty()
{
    int t = _isatty(0);
    if (t == 0)
        return false;
    else
        return true;
}
