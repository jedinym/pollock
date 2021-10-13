#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct pixel {
    unsigned short r;
    unsigned short g;
    unsigned short b;
} pixel_t;


typedef struct screen {
    pixel_t **pixels;
    unsigned width;
    unsigned height;
} screen_t;

bool create_screen(screen_t *screen, unsigned width, unsigned height);
void destroy_screen(screen_t *screen);

bool init_window_renderer(SDL_Window **window,
                          SDL_Renderer **renderer,
                          int width, int heigth);


int render(void *render_data);

int render_screen_frame(screen_t screen, SDL_Renderer *renderer);

typedef struct render_data {
    SDL_Renderer *renderer;
    screen_t screen;
} render_data_t;

#endif //RENDER_H
