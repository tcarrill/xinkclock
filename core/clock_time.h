#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

typedef struct {
    int hour;
    int minute;
    int second;
} ClockTime;

ClockTime get_current_time(void);

#endif
