#include "../core/xclock.h"
#include "../render/render.h"
#include "trig_table.h"

#define FIX_SHIFT 10   /* 1024 scaling */

/* Geometry ratios (scaled by 1024 for precision) */
#define RADIUS_RATIO        461   /* 0.45  * 1024 */
#define HOUR_LEN_RATIO      512   /* 0.50  * 1024 */
#define MIN_LEN_RATIO       768   /* 0.75  * 1024 */
#define HOUR_BASE_RATIO     160   /* 0.075 * 1024 */
#define MIN_BASE_RATIO      120   /* 0.055 * 1024 */

/* Multiply by ratio and divide by 1024 using shift */
#define SCALE(val, ratio) (((val) * (ratio)) >> FIX_SHIFT)

// Predefined constants (no division at runtime)
#define MINUTE_TICK_LEN  8
#define HOUR_TICK_LEN   16

#define WIDTH 400
#define HEIGHT 300

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
    int pivot_x, int pivot_y,
    int joint_x, int joint_y,
    int angle,
    int length,
    int base_width
)
{
    /* Tip (Q1.15 * int -> int) */
    int tip_x = pivot_x + ((COS(angle) * length) >> 15);
    int tip_y = pivot_y + ((SIN(angle) * length) >> 15);

    int half = base_width >> 1;

    /* Perpendicular vector */
    int perp_x = -SIN(angle);
    int perp_y =  COS(angle);

    int dx = (perp_x * half) >> 15;
    int dy = (perp_y * half) >> 15;

    int left_x  = joint_x + dx;
    int left_y  = joint_y + dy;
    int right_x = joint_x - dx;
    int right_y = joint_y - dy;

    r->draw_filled_triangle(
        left_x, left_y,
        right_x, right_y,
        tip_x, tip_y
    );
}

void xclock_draw(Render *r, ClockTime t)
{
    int cx = WIDTH  >> 1;
    int cy = HEIGHT >> 1;

    int min_dim = (WIDTH < HEIGHT) ? WIDTH : HEIGHT;

    /* Radius */
    int radius = SCALE(min_dim, RADIUS_RATIO);

    /* Angles (no division) */
    int minute_angle = t.minute * 6 - 90;
    int hour_angle   = (t.hour % 12) * 30
                     + (t.minute * 30 >> 6)  /* /60 via shift+mul */
                     - 90;

    /* Hand lengths */
    int min_length  = SCALE(radius, MIN_LEN_RATIO);
    int hour_length = SCALE(radius, HOUR_LEN_RATIO);

    /* Hand bases */
    int min_base  = SCALE(radius, MIN_BASE_RATIO);
    int hour_base = SCALE(radius, HOUR_BASE_RATIO);

    r->clear();

    for (int i = 0; i < 60; i++) {
        /* Angle in degrees */
        int angle = i * 6 - 90;

        /* Outer point */
        int outer_x = cx + ((COS(angle) * radius) >> 15);
        int outer_y = cy + ((SIN(angle) * radius) >> 15);

        int tick_len = tick_len_table[i];

        int inner_r = radius - tick_len;

        int inner_x = cx + ((COS(angle) * inner_r) >> 15);
        int inner_y = cy + ((SIN(angle) * inner_r) >> 15);

        r->draw_line(inner_x, inner_y, outer_x, outer_y);
    }

    /* Minute hand (draw first) */
    draw_hand_triangle(
        r,
        cx, cy + 5,     /* pivot */
        cx, cy - 1,     /* joint */
        minute_angle,
        min_length,
        min_base
    );

    /* Hour hand */
    draw_hand_triangle(
        r,
        cx, cy - 5,     /* pivot */
        cx, cy + 1,     /* joint */
        hour_angle,
        hour_length,
        hour_base
    );

    r->flush();
}
