#include <stdio.h>
#include <stdlib.h>

#include "flora_fonts.h"
#include "flora_apps.h"
#include "flora_constants.h"

bool create_text_widget(FloraApplicationState *state) {
}

bool init_fonts(FloraApplicationState *state) {
    if (!TTF_Init()) {
        fprintf(stderr, "Error: Failed to initialize fonts: %s\n", SDL_GetError());
        return false;
    }

    state->font_capacity = INITIAL_FONT_CAPACITY;

    state->fonts = malloc(sizeof(TTF_Font *) * state->font_capacity);
    if (!state->fonts) {
        fprintf(stderr, "Error: Failed to allocate new fonts array. Out of memory.\n");
        return false;
    }

    return true;
}

bool destroy_fonts(FloraApplicationState *state) {
    for (int i = 0; i < state->font_count; i++) {
        TTF_Font *font = state->fonts[i];
        if (font) TTF_CloseFont(font);
    }
    state->font_count = 0;
    state->font_capacity = 0;
    free(state->fonts);

    TTF_Quit();
    printf("Log: Successfully destroyed fonts.\n");
    return true;
}

bool add_font(FloraApplicationState *state, const char *path, float point_size) {
    if (!path) {
        fprintf(stderr, "Error: Font path not provided.\n");
        return false;
    }
    if (!state) {
        fprintf(stderr, "Error: Application not initialized.\n");
        return false;
    }

    TTF_Font *font = TTF_OpenFont(path, point_size);
    if (!font) {
        fprintf(stderr, "Error: Failed to open font \"%s\".\n", path);
        return false;
    }

    if (state->font_count == state->font_capacity) {
        TTF_Font **new_fonts = realloc(state->fonts, state->font_capacity * GROWTH_FACTOR * sizeof(TTF_Font *));
        if (!new_fonts) {
            fprintf(stderr, "Error: Failed to allocate new fonts array. Out of memory.\n");
            return false;
        }
        state->fonts = new_fonts;
    }
    state->fonts[state->font_count++] = font;
    return true;
}
