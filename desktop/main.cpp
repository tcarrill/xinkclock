#include "../core/xclock.h"
#include "../render/render.h"
#include "../core/clock_time.h"
#include <SDL2/SDL.h>

int main(void)
{
    Render r = sdl_renderer(window_width, window_height); 

    // Simple SDL loop to keep window open
    SDL_Event e;
    int running = 1;
    Uint32 last_tick = 0;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last_tick >= 1000) {
            last_tick = now;
            ClockTime t = get_current_time();
            xclock_draw(&r, t);
        }
        SDL_Delay(10);
    }

    return 0;
}
