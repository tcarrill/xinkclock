#ifndef XCLOCK_H
#define XCLOCK_H

#include "clock_time.h"
#include "../render/render.h"

inline constexpr int window_width = 400;
inline constexpr int window_height = 300;
inline constexpr int radius = (window_width < window_height ? window_width : window_height) / 2 - 10;

void xclock_draw(Render *r, ClockTime t);

#endif