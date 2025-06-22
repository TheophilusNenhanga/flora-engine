#include "flora_widget.h"
#include "app_state.h"

void baseWidgetRender(FloraWidget* widget, ApplicationState* state) {
	SDL_SetRenderDrawColor(state->mainRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	const SDL_FRect rect = { widget->posX, widget->posY, widget->width, widget->height };
	SDL_RenderFillRect(state->mainRenderer, &rect);
}

void baseWidgetUpdate(FloraWidget* widget, ApplicationState* state) {
	widget->posX += 30 * (float)state->deltaTime;
	if (widget->posX > state->windowWidth) {
		widget->posX = 0;
	}
	widget->posY += 30 * (float)state->deltaTime;
	if (widget->posY > state->windowHeight) {
		widget->posY = 0;
	}
}