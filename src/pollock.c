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


static int get_random_screen(void *screen_data) {
    screen_t *screen = (screen_t *) screen_data;
    srand(time(NULL));

    for (int x = 0; x < screen->width; ++x) {
        for (int y = 0; y < screen->height; ++y) {
            pixel_t pixel = {(x*y) % 256, (x*y) % 256, (x*y) % 256};
            screen->pixels[x][y] = pixel;
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

    //main rendering loop
    while (keep_rendering) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_rendering = false;
            }
        }

        thrd_t thrd;
        thrd_create(&thrd, get_random_screen, &screen);

        render_screen_frame(screen, renderer);

        thrd_join(thrd, NULL);
    }


    destroy_screen(&screen);
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return EXIT_SUCCESS;
}

