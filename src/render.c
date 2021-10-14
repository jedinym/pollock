#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#include "pollock.h"
#include "render.h"

#define SEC_TO_MS(sec) ((sec)*1000)
#define NS_TO_MS(ns) ((ns)/1000000)

unsigned long long last_frame_time = 0;

static unsigned long long millis()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    uint64_t ms = SEC_TO_MS((uint64_t)ts.tv_sec) + NS_TO_MS((uint64_t)ts.tv_nsec);
    return ms;
}

bool init_window_renderer(SDL_Window **window,
                          SDL_Renderer **renderer,
                          int width, int heigth) 
{
    SDL_Window *tmp_window = SDL_CreateWindow("pollock", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width, heigth,
                                          SDL_WINDOW_SHOWN);
    if (tmp_window == NULL) {
        return false;
    }

    SDL_Renderer *tmp_renderer = SDL_CreateRenderer(tmp_window, 0, 0);
    if (tmp_renderer == NULL) {
        SDL_DestroyWindow(tmp_window);
        return false;
    }

    *window = tmp_window;
    *renderer = tmp_renderer;
    return true;
}

bool create_screen(screen_t *screen, unsigned width, unsigned height) {
    pixel_t **pixels = malloc(width * sizeof(pixel_t*));
    if (pixels == NULL) {
        perror("malloc error");
        return false;
    }

    for (int i = 0; i < width; ++i) {
        pixels[i] = malloc(height * sizeof(pixel_t));
        if (pixels[i] == NULL) {
            perror("malloc error");
            for (int j = 0; j < i; ++j) {
                free(pixels[i]);
            }
            free(pixels);
            return false;
        }
    }

    screen->pixels = pixels;
    screen->width = width;
    screen->height = height;

    return true;
}


void destroy_screen(screen_t *screen) {
    for (int i = 0; i < screen->width; ++i) {
        free(screen->pixels[i]);
    }
    free(screen->pixels);
}


int render_screen_frame(screen_t screen, SDL_Renderer *renderer) {
    unsigned long long now = millis();
    for (unsigned x = 0; x < screen.width; ++x) {
        for (unsigned y = 0; y < screen.height; ++y) {
            pixel_t *pixel = &screen.pixels[x][y];
            SDL_SetRenderDrawColor(renderer, pixel->r, pixel->g, pixel->b, 0xff);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
    printf("%.0lf FPS\n", ((float)1 / (millis() - now)) * 1000);
    return 0;
}
