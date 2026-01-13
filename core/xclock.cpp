#include "xclock.h"
#include <math.h>

#define DEG2RAD(d) ((d) * M_PI / 180.0f)
#define HOUR_TICK_LEN 15
#define MIN_TICK_LEN 6
#define CX window_width / 2
#define CY window_height / 2
#define HOUR_LENGTH radius * 0.72f
#define MINUTE_LENGTH radius * 0.47f
#define HOUR_WIDTH 20
#define MINUTE_WIDTH 20

static void draw_hand_triangle(Render *r, float angle_deg, int length, int width)
{
    float rad = DEG2RAD(angle_deg);

    // Tip
    int tip_x = CX + cosf(rad) * length;
    int tip_y = CY + sinf(rad) * length;

    // Base center offset forward
    int base_x = CX; //+ cosf(rad) * base_offset;
    int base_y = CY; //+ sinf(rad) * base_offset;

    // Asymmetric base
    float lw = width * 0.35f;
    float rw = width * 0.65f;

    int left_x  = base_x + cosf(rad + M_PI/2) * lw;
    int left_y  = base_y + sinf(rad + M_PI/2) * lw;

    int right_x = base_x + cosf(rad - M_PI/2) * rw;
    int right_y = base_y + sinf(rad - M_PI/2) * rw;

    r->draw_filled_triangle(left_x, left_y, right_x, right_y, tip_x, tip_y);
}


void xclock_draw(Render *r, ClockTime t)
{
    r->clear();

    // Draw tick marks
    for (int i = 0; i < 60; i++) {
        float angle = i * 6.0f - 90.0f; // start at 12 o'clock
        float rad = DEG2RAD(angle);

        int outer_x = CX + cosf(rad) * radius;
        int outer_y = CY + sinf(rad) * radius;

        int tick_len = (i % 5 == 0) ? HOUR_TICK_LEN : MIN_TICK_LEN;
        int inner_x = CX + cosf(rad) * (radius - tick_len);
        int inner_y = CY + sinf(rad) * (radius - tick_len);

        r->draw_line(inner_x, inner_y, outer_x, outer_y);
    }

    // Calculate angles
    float hour_angle = (t.hour % 12 + t.minute / 60.0f) * 30.0f - 90.0f;
    float minute_angle  = t.minute * 6.0f - 90.0f;

    // Minute hand first (behind)
    draw_hand_triangle(r, minute_angle, MINUTE_LENGTH, MINUTE_WIDTH);

    // Hour hand second (on top)
    draw_hand_triangle(r, hour_angle, HOUR_LENGTH, HOUR_WIDTH);

    r->flush();
}

