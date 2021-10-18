#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <iostream>

#include "render.hpp"

#define SEC_TO_MS(sec) ((sec)*1000)
#define NS_TO_MS(ns) ((ns)/1000000)

unsigned long long last_frame_time = 0;

// Pixel definitions
Pixel::Pixel(short _r, short _g, short _b) {
    r = _r;
    g = _g;
    b = _b;
}

Pixel::Pixel() {
    r = 0;
    g = 0;
    b = 0;
}

std::tuple<short, short, short> Pixel::get_color() {
    return std::make_tuple(r, g, b);
}

// Screen definitions
Screen::Screen(unsigned _width, unsigned _height) {
    width = _width;
    height = _height;
    pixels = new Pixel*[width];
    for (int i = 0; i < width; ++i) {
        pixels[i] = new Pixel[height];
    }
}

Screen::~Screen() {
    for (int i = 0; i < width; ++i) {
        delete pixels[i];
    }
    delete[] pixels;
}

unsigned Screen::get_width() {
    return width;
}

unsigned Screen::get_height() {
    return height;
}

Pixel* Screen::get_pixel(unsigned x, unsigned y) {
    return &pixels[x][y];
}

void Screen::set_pixel(unsigned x, unsigned y, Pixel pixel) {
    pixels[x][y] = pixel;
}

////////////////

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

int render_screen_frame(Screen &screen, SDL_Renderer *renderer) {
    unsigned long long now = millis();
    for (unsigned x = 0; x < screen.get_width(); ++x) {
        for (unsigned y = 0; y < screen.get_height(); ++y) {
            std::tuple<short, short, short> color = screen.get_pixel(x, y)->get_color();
            SDL_SetRenderDrawColor(renderer, std::get<0>(color),
                    std::get<1>(color), std::get<2>(color), 0xff);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
    printf("%.0lf FPS\n", ((float)1 / (millis() - now)) * 1000);
    return 0;
}

