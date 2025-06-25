#ifndef FLORA_WIDGET_H
#define FLORA_WIDGET_H

#include <SDL.h>
#include <stdbool.h>

struct ApplicationState;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} FloraColour;

typedef struct {
	FloraColour colour;
	FloraColour borderColour;
} FloraWidgetStyle;

typedef struct FloraWidget {
	float posX;
	float posY;
	float width;
	float height;
	FloraWidgetStyle style;
	void (*update)(struct FloraWidget* widget, struct ApplicationState* state);
	void (*render)(struct FloraWidget* widget, struct ApplicationState* state);
	void (*onClick)(struct FloraWidget* widget, struct ApplicationState* state, SDL_MouseButtonEvent* event);
	bool isVisible;
} FloraWidget;

void baseWidgetRender(FloraWidget* widget, struct ApplicationState* state);
void baseWidgetUpdate(FloraWidget* widget, struct ApplicationState* state);
void baseWidgetOnClick(FloraWidget* widget, struct ApplicationState* state, SDL_MouseButtonEvent* event);

bool widgetContainsPoint(FloraWidget* widget, float x, float y);
#endif
