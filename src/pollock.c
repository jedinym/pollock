#include <stdio.h>
#include <threads.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include "render.h"
#include "pollock.h"

const int WIDTH = 800;
const int HEIGTH = 600;

volatile bool keep_rendering = true;
volatile bool render_next = false;

static pixel_t sum_pixels(pixel_t *a, pixel_t *b) {
    pixel_t summed;
    summed.r = a->r + b->r;
    summed.g = a->g + b->g;
    summed.b = a->b + b->b;
    return summed;
}


static int get_random_screen(void *screen_data) {
    screen_t *screen = (screen_t *) screen_data;
    srand(time(NULL));

    for (int x = 0; x < screen->width; ++x) {
        int v = rand() % 150;
        pixel_t addon = {v, v, v};
        for (int y = 0; y < screen->height; ++y) {
            pixel_t old = screen->pixels[x][y];
            pixel_t new = sum_pixels(&old, &addon);
            int randr = rand() % 56;
            new.r %= 20 + randr;
            new.g %= 20 + randr;
            new.b %= 20 + randr;
            // pixel_t pixel = {(x+y) % 256, (x+y) % 256, (x+y) % 256};
            screen->pixels[x][y] = new;
        }
    }

    return 0;
}


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

    screen_t screen;
    if (!create_screen(&screen, WIDTH, HEIGTH)) {
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        SDL_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    // which function to use for generating rendered data
    int (*generator_function)(void*) = &get_random_screen;

    //main rendering loop
    while (keep_rendering) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_rendering = false;
            }
        }

        thrd_t thrd;
        // calling the function to generate data
        thrd_create(&thrd, generator_function, &screen);

        render_screen_frame(screen, renderer);

        int return_val;
        thrd_join(thrd, &return_val);
        if (return_val != 0) {
            destroy_screen(&screen);
            SDL_Quit();
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
        }
    }


    destroy_screen(&screen);
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return EXIT_SUCCESS;
}

