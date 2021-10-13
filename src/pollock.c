#include <stdio.h>
#include <threads.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>

#include "render.h"

const int WIDTH = 1024;
const int HEIGTH = 768;


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

    pixel_t test_pixel = {.r = 38, .g = 226, .b = 255};
    pixel_t bad_pixel = {.r = 200, .g = 50, .b = 0};

    screen_t screen;
    if (!create_screen(&screen, WIDTH, HEIGTH)) {
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        SDL_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGTH; ++j) {
            pixel_t pixel = {.r = rand() % 256, .g = rand() % 256, .b = rand() % 256};
            screen.pixels[i][j] = pixel;
        }
    }

    render_data_t render_data = {.renderer = renderer, .screen = screen};
    render(&render_data);

    // SDL_Delay(5000);

    destroy_screen(&screen);
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return EXIT_SUCCESS;
}
