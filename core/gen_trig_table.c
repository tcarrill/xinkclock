#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define SCALE 32767.0   // Q1.15 max value

// compile: cc gen_trig_table.c -lm -o gen_trig_table
int main(void)
{
    printf("#include \"trig_table.h\"\n\n");

    /* --- sine table --- */
    printf("const int16_t sin_table[360] = {\n");
    for (int i = 0; i < 360; i++) {
        double rad = i * M_PI / 180.0;
        int val = (int)round(sin(rad) * SCALE);

        if (i % 8 == 0) printf("    ");
        printf("%6d", val);
        if (i != 359) printf(", ");
        if (i % 8 == 7) printf("\n");
    }
    printf("\n};\n\n");

    /* --- cosine table --- */
    printf("const int16_t cos_table[360] = {\n");
    for (int i = 0; i < 360; i++) {
        double rad = i * M_PI / 180.0;
        int val = (int)round(cos(rad) * SCALE);

        if (i % 8 == 0) printf("    ");
        printf("%6d", val);
        if (i != 359) printf(", ");
        if (i % 8 == 7) printf("\n");
    }
    printf("\n};\n");

    return 0;
}
