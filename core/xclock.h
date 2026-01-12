#ifndef XCLOCK_H
#define XCLOCK_H

#include "clock_time.h"
#include "../render/render.h"

void xclock_draw(Render *r, ClockTime t, int window_width, int window_height);

#endif