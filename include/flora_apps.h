#ifndef FLORA_APPS_H
#define FLORA_APPS_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdint.h>

#include "flora_widgets.h"

struct FloraApplicationState {
    SDL_Window *window;
    SDL_Renderer *renderer;

    TTF_Font *font;
    float font_size;

    uint64_t last_frame_time;
    double delta_time;
    int window_width;
    int window_height;
    bool running;
    EventQueue event_queue;
    FloraScreen *current_screen;
};

struct FloraScreen {
    void (*on_screen_create)(FloraApplicationState *state, FloraScreen *screen);

    void (*on_screen_destroy)(FloraApplicationState *state, FloraScreen *screen);

    FloraWidget *widgets;
    int widget_count;
    int widget_capacity;
    // TODO: Allow for multiple screens
    // int screen_id
    // char* screen_name
    // int screen_name_length
    // FloraScreen* next_screen;
};


bool init_application(FloraApplicationState *state, const char *title, int width, int height);

bool destroy_application(FloraApplicationState *state);

bool create_window(FloraApplicationState *state, const char *title);

bool destroy_window(FloraApplicationState *state);

#endif //FLORA_APPS_H
