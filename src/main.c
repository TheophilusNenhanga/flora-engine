#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "flora_constants.h"
#include "flora_apps.h"
#include "flora_events.h"
#include "flora_screens.h"

void application_loop(FloraApplicationState* state) {
    FloraScreen* screen = state->current_screen;
    while (state->running) {
        const uint64_t frame_start = SDL_GetTicks();
        get_input(state);
        update_screen(screen, state);
        SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
        SDL_RenderClear(state->renderer);
        render_screen(screen, state);
        SDL_RenderPresent(state->renderer);

        const uint64_t frame_end = SDL_GetTicks();

        state->delta_time = (frame_end - frame_start) / 1000.0f;
        state->last_frame_time = frame_end;
    }
}

int main(const int argc, char** argv) {
    int width = 800;
    int height = 600;
    char* title = "Flora Engine";
    if (argc < 4) {
        printf("Alternate Usage: %s <width> <height> <title>\n", argv[0]);
        printf("Using: %s %d %d %s\n", argv[0], width, height, "Flora Engine");
    }else {
        width = strtol(argv[1], NULL, 10);
        height = strtol(argv[2], NULL, 10);
        title = argv[3];
    }


    FloraApplicationState state = {
        .renderer = NULL,
        .window = NULL,
        .last_frame_time = 0,
        .delta_time = 0.0f,
        .running = false,
        .window_width = width,
        .window_height = height,
        .event_queue = {0},
        .current_screen = NULL
    };

    init_application(&state, title, state.window_width, state.window_height);
    state.running = true;
    application_loop(&state);

    if (!destroy_application(&state)) {
        fprintf(stderr, "Error: Failed to destroy application\n");
        return FLORA_ENGINE_FATAL;
    }
    return FLORA_ENGINE_SUCCESS;
}
