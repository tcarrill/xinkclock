#include "xclock.h"
#include "trig_table.h"

#define FIX_SHIFT 10

#define RADIUS_RATIO        461
#define HOUR_LEN_RATIO      512
#define MIN_LEN_RATIO       768
#define HOUR_BASE_RATIO     160
#define MIN_BASE_RATIO      120

#define SCALE(val, ratio) (((val) * (ratio)) >> FIX_SHIFT)

#ifndef XCLOCK_CX_BIAS
#define XCLOCK_CX_BIAS 0
#endif

#ifndef XCLOCK_CY_BIAS
#define XCLOCK_CY_BIAS 8
#endif

static const uint8_t tick_len_table[60] = {
    16,8,8,8,8, 16,8,8,8,8,
    16,8,8,8,8, 16,8,8,8,8,
    16,8,8,8,8, 16,8,8,8,8,
    16,8,8,8,8, 16,8,8,8,8,
    16,8,8,8,8, 16,8,8,8,8,
    16,8,8,8,8, 16,8,8,8,8
};

static void draw_hand_triangle(
    Render *r,
    int cx, int cy,
    int angle,
    int length,
    int base_width)
{
    int tip_x = cx + ((COS(angle) * length) >> 15);
    int tip_y = cy + ((SIN(angle) * length) >> 15);

    int half = base_width >> 1;
    int perp_x = -SIN(angle);
    int perp_y =  COS(angle);

    int dx = (perp_x * half) >> 15;
    int dy = (perp_y * half) >> 15;

    int left_x  = cx + dx;
    int left_y  = cy + dy;
    int right_x = cx - dx;
    int right_y = cy - dy;

    r->draw_filled_triangle(left_x, left_y, right_x, right_y, tip_x, tip_y);
}

void xclock_draw(Render *r, ClockTime t)
{
    const int W = (r && r->width)  ? r->width  : 400;
    const int H = (r && r->height) ? r->height : 300;

    const int cx = (W >> 1) + XCLOCK_CX_BIAS;
    const int cy = (H >> 1) + XCLOCK_CY_BIAS;

    const int min_dim = (W < H) ? W : H;
    const int radius  = SCALE(min_dim, RADIUS_RATIO);

    const int minute_angle = t.minute * 6 - 90;
    const int hour_angle   = (t.hour % 12) * 30 + (t.minute * 30) / 60 - 90;

    const int min_length  = SCALE(radius, MIN_LEN_RATIO);
    const int hour_length = SCALE(radius, HOUR_LEN_RATIO);

    int min_base  = SCALE(radius, MIN_BASE_RATIO);
    int hour_base = SCALE(radius, HOUR_BASE_RATIO);
    if (min_base  < 6) min_base  = 6;
    if (hour_base < 8) hour_base = 8;

    r->clear();

    for (int i = 0; i < 60; i++)
    {
        const int angle = i * 6 - 90;

        const int outer_x = cx + ((COS(angle) * radius) >> 15);
        const int outer_y = cy + ((SIN(angle) * radius) >> 15);

        const int tick_len = tick_len_table[i];
        const int inner_r  = radius - tick_len;

        const int inner_x = cx + ((COS(angle) * inner_r) >> 15);
        const int inner_y = cy + ((SIN(angle) * inner_r) >> 15);

        r->draw_line(inner_x, inner_y, outer_x, outer_y);
    }

    draw_hand_triangle(r, cx, cy, hour_angle, hour_length, hour_base);
    draw_hand_triangle(r, cx, cy, minute_angle, min_length, min_base);

    r->flush();
}
