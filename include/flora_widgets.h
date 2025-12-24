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

typedef struct {
    FloraColour inner_colour;
    FloraColour border_colour;
    FloraPadding padding;
    FloraGap gap;
}FloraWidgetStyle;

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

typedef struct FloraWidget FloraWidget;

typedef void (*widget_update)(FloraWidget* widget, FloraApplicationState* state);
typedef void (*widget_render)(FloraWidget* widget, FloraApplicationState* state);
typedef void (*widget_on_mouse_down)(FloraWidget* widget, FloraApplicationState* state);

struct FloraWidget {
    int id;
    FloraWidgetType type;
    FloraPosition position;
    FloraWidgetStyle style;
    FloraSizing sizing;
    FloraWidget* parent;
    FloraWidget** children;
    FloraLayoutDirection layout_direction;
    int child_count;
    int child_capacity;
    bool is_visible;
    widget_update update;
    widget_render render;
    widget_on_mouse_down on_mouse_down;

    union {
        struct {

        }box;
        struct {
            int font_index;
            int font_size;
            FloraColour font_colour;
            FloraColour font_background;
            char* content;
        }text;
    }as;

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


bool destroy_flora_widget(FloraWidget* widget);

bool widget_contains_point(FloraWidget* widget, int x, int y);

void base_widget_render(FloraWidget* widget, FloraApplicationState* state);
void base_widget_update(FloraWidget* widget, FloraApplicationState* state);
void base_widget_on_mouse_down(FloraWidget* widget, FloraApplicationState* state);

#endif //FLORA_WIDGETS_H
