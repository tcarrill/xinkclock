#ifndef TRIG_TABLE_H
#define TRIG_TABLE_H

#include <stdint.h>

/* Q1.15 fixed-point trig tables, indexed 0-359 degrees.
 * Values range from -32767 to +32767 representing -1.0 to +1.0.
 * Multiply result * distance, then >> 15 to get pixel offset. */
extern const int16_t sin_table[360];
extern const int16_t cos_table[360];

/* Safe lookup with wrap-around for negative angles */
#define SIN(deg) (sin_table[((deg) + 360) % 360])
#define COS(deg) (cos_table[((deg) + 360) % 360])

#endif
