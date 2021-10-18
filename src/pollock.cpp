#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>

#include "render.hpp"
#include "pollock.hpp"

const int WIDTH = 600;
const int HEIGTH = 600;

volatile bool keep_rendering = true;
volatile bool render_next = false;

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init error %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!init_window_renderer(&window, &renderer, WIDTH, HEIGTH)) {
        fprintf(stderr, "Init window/renderer error\n");
        return EXIT_FAILURE;
    }

    Screen screen (WIDTH, HEIGTH);

    // zima blue
    Pixel pix(94, 223, 255);

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGTH; ++y) {
            screen.set_pixel(x, y, pix);
        }
    }

    //main rendering loop
    while (keep_rendering) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_rendering = false;
            }
        }

        render_screen_frame(screen, renderer);
    }

    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return EXIT_SUCCESS;
}

