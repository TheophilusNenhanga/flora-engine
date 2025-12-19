#ifndef FLORA_SCREENS_H
#define FLORA_SCREENS_H

#include "flora_apps.h"

bool destroy_flora_screen(FloraScreen* screen);
void update_screen(FloraScreen* screen, FloraApplicationState* state);
void render_screen(FloraScreen* screen, FloraApplicationState* state);
bool init_flora_screen(FloraScreen* screen, FloraApplicationState* state);


#endif //FLORA_SCREENS_H
