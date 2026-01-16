#ifndef TRIG_TABLE_H
#define TRIG_TABLE_H

#include <stdint.h>

/* Q1.15 fixed-point trig tables */
extern const int16_t sin_table[360];
extern const int16_t cos_table[360];

#define SIN(deg) (sin_table[((deg) + 360) % 360])
#define COS(deg) (cos_table[((deg) + 360) % 360])

#endif
