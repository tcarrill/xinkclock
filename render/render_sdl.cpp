#include "render_sdl.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdexcept>
#include <cstdio>

RendererSDL::RendererSDL(int windowWidth, int windowHeight)
    : width_(windowWidth), height_(windowHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    window_ = SDL_CreateWindow(
        "xclock",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window_) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer_) {
        throw std::runtime_error(SDL_GetError());
    }

    // Default drawing color = black
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
}

int RendererSDL::width() const { return width_; }
int RendererSDL::height() const { return height_; }

RendererSDL::~RendererSDL()
{
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
    }

    if (window_) {
        SDL_DestroyWindow(window_);
    }

    SDL_Quit();
}

void RendererSDL::clear()
{
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
}

void RendererSDL::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

void RendererSDL::drawFilledTriangle(
    int x0, int y0,
    int x1, int y1,
    int x2, int y2)
{
    filledTrigonRGBA(
        renderer_,
        x0, y0,
        x1, y1,
        x2, y2,
        0, 0, 0, 255
    );
}

void RendererSDL::flush()
{
    SDL_RenderPresent(renderer_);
}