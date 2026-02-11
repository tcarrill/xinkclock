#ifndef RENDER_H
#define RENDER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Render {
    int width;
    int height;

    void (*clear)(void);
    void (*draw_filled_triangle)(int x0, int y0, int x1, int y1, int x2, int y2);
    void (*draw_line)(int x1, int y1, int x2, int y2);
    void (*flush)(void);

    void* userdata;
} Render;

Render gxepd2_renderer(void);

#ifdef __cplusplus
}
#endif

#endif
