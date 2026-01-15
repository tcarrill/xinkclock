#include "render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdio.h>

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;

static void clear(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

static void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2) {
    filledTrigonRGBA(renderer, x0, y0, x1, y1, x2, y2, 0, 0, 0, 255);
}

static void draw_line(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

static void flush(void) {
    SDL_RenderPresent(renderer);
}

Render sdl_renderer(int window_width, int window_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow(
        "xclock",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Render r = {
        .clear = clear,
        .draw_filled_triangle = draw_filled_triangle,
        .draw_line = draw_line,
        .flush = flush
    };
    return r;
}
