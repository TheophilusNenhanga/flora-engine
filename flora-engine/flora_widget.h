#ifndef FLORA_WIDGET_H
#define FLORA_WIDGET_H

#include <SDL.h>
#include <stdbool.h>

#include "flora_colours.h"

struct ApplicationState;

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
} FloraLayoutDirection;

typedef enum {
	FIT = 0,
	FIXED,
	GROW,
} FloraSizingType;

typedef struct {
	FloraSizingType type;
	float value;
} FloraWidth;

typedef struct {
	FloraSizingType type;
	float value;
} FloraHeight;

typedef struct {
	FloraWidth width;
	FloraHeight height;
} FloraSizing;

#define FLORA_WIDTH(sizingType, width) \
	((FloraWidth){.type = sizingType, .value = width})

#define FLORA_HEIGHT(sizingType, height) \
	((FloraHeight){.type = sizingType, .value = height})	

#define FLORA_HEIGHT_FIT(value) \
	FLORA_HEIGHT(FIT, value)

#define FLORA_HEIGHT_FIXED(value) \
	FLORA_HEIGHT(FIXED, value)

#define FLORA_HEIGHT_GROW(value) \
	FLORA_HEIGHT(GROW, value)


#define FLORA_WIDTH_FIT(value) \
	FLORA_WIDTH(FIT, value)

#define FLORA_WIDTH_FIXED(value) \
	FLORA_WIDTH(FIXED, value)

#define FLORA_WIDTH_GROW(value) \
	FLORA_WIDTH(GROW, value)

typedef struct {
	float x;
	float y;
} FloraPosition;


typedef struct FloraWidget FloraWidget;

struct FloraWidget {
	uint16_t id;
	FloraPosition position;
	FloraWidgetStyle style;
	FloraLayoutDirection layoutDirection;
	FloraSizing sizing;
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