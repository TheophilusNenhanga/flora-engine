#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "flora_apps.h"
#include "SDL3/SDL_init.h"
#include "flora_constants.h"
#include "flora_events.h"
#include "flora_fonts.h"
#include "flora_screens.h"

bool create_window(FloraApplicationState* state, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    state->window = SDL_CreateWindow(title, state->window_width, state->window_height, 0);
    if (!state->window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(state->window);
        SDL_Quit();
        return false;
    }
    printf("Log: Window created successfully\n");
    return true;
}

bool destroy_window(FloraApplicationState* state) {
    if (!state->window) {
        fprintf(stderr, "Error: Window is not initialized\n");
        return false;
    }
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    state->window = NULL;
    state->renderer = NULL;
    state->running = false;

    if (!destroy_event_queue(&state->event_queue)) {
        fprintf(stderr, "Error: Failed to destroy event queue\n");
        return false;
    }

    printf("Log: Window destroyed successfully\n");
    return true;
}

bool init_application(FloraApplicationState* state, const char* title, int width, int height) {
    state->window_width = width;
    state->window_height = height;
    if (!create_window(state, title)) {
        fprintf(stderr, "Error: Failed to create window\n");
        return false;
    }

    state->current_screen = malloc(sizeof(FloraScreen));
    if (!state->current_screen) {
        fprintf(stderr, "Error: Failed to allocate memory for current screen\n");
        destroy_window(state);
        return false;
    }
    if (!init_flora_screen(state->current_screen, state)) {
        fprintf(stderr, "Error: Failed to initialize current screen\n");
        return false;
    }
    if (!init_event_queue(&state->event_queue, 64)) {
        fprintf(stderr, "Error: Failed to initialize event queue\n");
        return false;
    }

    if (!init_fonts()) {
        return false;
    }

    if (!add_font(state, OPEN_SANS_FONT_PATH, 24)) {
        fprintf(stderr, "Error: Failed to add font\n");
        return false;
    }

    if (state->current_screen->on_screen_create) {
        state->current_screen->on_screen_create(state, state->current_screen);
    }

    printf("Log: Application state initialized successfully\n");
    return true;
}

bool destroy_application(FloraApplicationState* state) {
    if (!destroy_window(state)) {
        fprintf(stderr, "Error: Failed to destroy window\n");
        return false;
    }
    if (!destroy_event_queue(&state->event_queue)) {
        fprintf(stderr, "Error: Failed to destroy event queue\n");
        return false;
    }

    for (int i = 0; i < state->font_count; i++) {
        if (state->fonts[i]) {
            TTF_CloseFont(state->fonts[i]);
        }
    }
    state->fonts = NULL;
    destroy_fonts();

    printf("Log: Application state destroyed successfully\n");
    return true;
}
