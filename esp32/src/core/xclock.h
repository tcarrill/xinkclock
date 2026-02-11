#ifndef XCLOCK_H
#define XCLOCK_H

#include "clock_time.h"
#include "../render/render.h"

#ifdef __cplusplus
extern "C" {
#endif

void xclock_draw(Render *r, ClockTime t);

#ifdef __cplusplus
}
#endif

#endif
