#ifndef RENDER_H
#define RENDER_H

typedef struct Render {
    void (*clear)(void);
    void (*draw_filled_triangle)(int x0, int y0, int x1, int y1, int x2, int y2);
    void (*draw_circle)(int cx, int cy, int r);
    void (*draw_line)(int x1, int y1, int x2, int y2);
    void (*flush)(void);
} Render;

Render sdl_renderer(int window_width, int window_height);

#endif