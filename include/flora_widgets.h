#ifndef FLORA_WIDGETS_H
#define FLORA_WIDGETS_H

#include <stdbool.h>
#include "flora_colours.h"
#include "flora_events.h"

typedef enum {
    FLORA_BOX,
    FLORA_TEXT
} FloraWidgetType;

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


typedef enum {
    LEFT_TO_RIGHT,
    TOP_TO_BOTTOM
} FloraLayoutDirection;

typedef enum {
    FIT,
    FIXED,
    GROW
} FloraSizingType;

typedef struct {
    FloraSizingType type;
    float value;
} FloraDimension;

typedef struct {
    FloraDimension width;
    FloraDimension height;
} FloraSizing;

typedef struct {
    float x;
    float y;
} FloraPosition;

typedef struct {
    FloraColour inner_colour;
    FloraColour border_colour;
    FloraPadding padding;
    FloraGap gap;
    FloraLayoutDirection layout_direction;
    FloraSizing sizing;
    FloraPosition position;
} FloraWidgetStyle;


typedef struct FloraWidget FloraWidget;

typedef void (*widget_update)(FloraWidget *widget, FloraApplicationState *state);

typedef void (*widget_render)(FloraWidget *widget, FloraApplicationState *state);

typedef void (*widget_on_mouse_down)(FloraWidget *widget, FloraApplicationState *state);

typedef void (*widget_on_destroy)(FloraWidget *widget, FloraApplicationState *state);

typedef struct {
    widget_update update;
    widget_render render;
    widget_on_mouse_down on_mouse_down;
    widget_on_destroy on_destroy;
} FloraWidgetCallbacks;

struct FloraWidget {
    int id;
    FloraWidgetType type;
    FloraWidget *parent;
    FloraWidget **children;
    FloraWidgetStyle style;
    FloraWidgetCallbacks callbacks;
    int child_count;
    int child_capacity;
    bool is_visible;

    union {
        struct {
        } box;

        struct {
            int font_index;
            int font_size;
            FloraColour font_colour;
            FloraColour font_background;
            char *content;
            int length; // length of the content
            SDL_Surface *surface;
        } text;
    } as;
};

#define FLORA_WIDTH(sizing_type, width) \
((FloraDimension){.type = sizing_type, .value = width})

#define FLORA_HEIGHT(sizing_type, height) \
((FloraDimension){.type = sizing_type, .value = height})

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


bool destroy_flora_widget(FloraWidget *widget);

bool widget_contains_point(FloraWidget *widget, int x, int y);

void base_box_widget_render(FloraWidget *widget, FloraApplicationState *state);

void base_box_widget_update(FloraWidget *widget, FloraApplicationState *state);

void base_box_widget_on_mouse_down(FloraWidget *widget, FloraApplicationState *state);

FloraWidget *create_box_widget(FloraApplicationState *state, FloraWidget *parent,
                               FloraWidgetStyle style, FloraWidgetCallbacks callbacks,
                               bool is_visible);

bool add_child_widget(FloraWidget *widget, FloraWidget *child);

#endif //FLORA_WIDGETS_H
