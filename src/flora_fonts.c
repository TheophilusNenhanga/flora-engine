#include <stdio.h>
#include <stdlib.h>

#include "flora_fonts.h"
#include "flora_apps.h"

bool init_fonts() {
    if (!TTF_Init()) {
        fprintf(stderr, "Error: Failed to initialize fonts: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool destroy_fonts(FloraApplicationState *state) {
    if (state->font) {
        TTF_CloseFont(state->font);
    }
    state->font = NULL;
    TTF_Quit();
    printf("Log: Successfully destroyed fonts.\n");
    return true;
}

TTF_Font* add_font(FloraApplicationState *state, const char *path, const float point_size) {
    if (!path) {
        fprintf(stderr, "Error: Font path not provided.\n");
        return NULL;
    }
    if (!state) {
        fprintf(stderr, "Error: Application not initialized.\n");
        return NULL;
    }

    TTF_Font *font = TTF_OpenFont(path, point_size);
    if (!font) {
        fprintf(stderr, "Error: Failed to open font \"%s\".\n", path);
        return NULL;
    }

    state->font = font;
    state->font_size = point_size;
    return font;
}
