#ifndef RENDER_H
#define RENDER_H

#include <tuple>
#include <SDL2/SDL.h>

class Pixel {
    short r;
    short g;
    short b;
public:
    Pixel(short _r, short _g, short _b);
    Pixel();
    std::tuple<short, short, short> get_color();
};

class Screen {
    unsigned width;
    unsigned height;
    Pixel **pixels;
public:
    Screen(unsigned _width, unsigned _height);
    ~Screen();
    unsigned get_width();
    unsigned get_height();
    Pixel *get_pixel(unsigned x, unsigned y);
    void set_pixel(unsigned x, unsigned y, Pixel pixel);
};

// typedef struct pixel {
//     unsigned short r;
//     unsigned short g;
//     unsigned short b;
// } pixel_t;


// typedef struct screen {
//     pixel_t **pixels;
//     unsigned width;
//     unsigned height;
// } screen_t;

// bool create_screen(screen_t *screen, unsigned width, unsigned height);
// void destroy_screen(screen_t *screen);

bool init_window_renderer(SDL_Window **window,
                          SDL_Renderer **renderer,
                          int width, int heigth);


int render_screen_frame(Screen& screen, SDL_Renderer *renderer);

// typedef struct render_data {
//     SDL_Renderer *renderer;
//     screen_t screen;
// } render_data_t;

#endif //RENDER_H
