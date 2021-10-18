#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>

#include "render.hpp"
#include "pollock.hpp"

const int WIDTH = 800;
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
    Pixel pix(100, 250, 50);

    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; i < HEIGTH; ++j) {
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

