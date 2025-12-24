#include <stdio.h>

#include "flora_fonts.h"
#include "flora_apps.h"

bool init_fonts() {
    if (!TTF_Init()) {
        fprintf(stderr, "Error: Failed to initialize fonts: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool destroy_fonts() {
    TTF_Quit();
    printf("Log: Successfully destroyed fonts.\n");
    return true;
}

bool add_font(FloraApplicationState* state, const char* path, float point_size) {
    if (!path) {
        fprintf(stderr, "Error: Font path not provided.\n");
        return false;
    }
    if (!state) {
        fprintf(stderr, "Error: Application not initialized.\n");
        return false;
    }

    TTF_Font* font = TTF_OpenFont(path, point_size);
    if (!font) {
        fprintf(stderr, "Error: Failed to open font \"%s\".\n", path);
        return false;
    }
    /* TODO: Add font to font array */
    return true;
}