#include "platform/linux.hpp"
#include "unistd.h"

QPoint os_active_window_pos()
{

}

bool os_isatty()
{
    int t = isatty(0);
    if (t == 0)
        return false;
    else
        return true;
}
