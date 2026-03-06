#pragma once

#include "render.h"
#include <SDL2/SDL.h>

class RendererSDL : public Renderer {
public:
    RendererSDL(int width, int height);
    ~RendererSDL();

    int width() const override;
    int height() const override;

    void clear() override;
    void drawLine(int x1, int y1, int x2, int y2) override;
    void drawFilledTriangle(
        int x0, int y0,
        int x1, int y1,
        int x2, int y2) override;

    void flush() override;

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int width_;
    int height_;
};