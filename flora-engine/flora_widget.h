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
	float left;
	float right;
	float top;
	float bottom;
} FloraPadding;

typedef struct {
	float x;
	float y;
} FloraGap;

typedef struct {
	FloraColour colour;
	FloraColour borderColour;
	FloraPadding padding;
	FloraGap gap;
} FloraWidgetStyle;

typedef enum {
	LEFT_TO_RIGHT,
	TOP_TO_BOTTOM,
} FloraLayout;

typedef struct FloraWidget FloraWidget;

struct FloraWidget {
	uint16_t id;
	float posX;
	float posY;
	float width;
	float height;
	FloraWidgetStyle style;
	FloraLayout layout;
	FloraWidget* parent;
	FloraWidget** children;
	int childCount;
	int childCapacity;

	void (*update)(struct FloraWidget* widget, struct ApplicationState* state);
	void (*render)(struct FloraWidget* widget, struct ApplicationState* state);
	void (*onClick)(struct FloraWidget* widget, struct ApplicationState* state, SDL_MouseButtonEvent* event);
	bool isVisible;
};

void baseWidgetRender(FloraWidget* widget, struct ApplicationState* state);
void baseWidgetUpdate(FloraWidget* widget, struct ApplicationState* state);
void baseWidgetOnClick(FloraWidget* widget, struct ApplicationState* state, SDL_MouseButtonEvent* event);

bool widgetContainsPoint(FloraWidget* widget, float x, float y);



#endif
