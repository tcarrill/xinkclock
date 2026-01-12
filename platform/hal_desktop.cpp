#include <time.h>
#include "../core/clock_time.h"

ClockTime get_current_time(void)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    ClockTime ct;
    ct.hour   = tm_info->tm_hour;
    ct.minute = tm_info->tm_min;
    ct.second = tm_info->tm_sec;
    return ct;
}
