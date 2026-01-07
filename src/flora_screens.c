#include <stdio.h>
#include <stdlib.h>

#include "flora_screens.h"
#include "flora_constants.h"

void base_screen_destroy(FloraApplicationState *state, FloraScreen *screen) {
    if (!screen) {
        fprintf(stderr, "Error: Screen is not initialized\n");
        return;
    }
    if (screen->widgets) {
        for (int i = 0; i < screen->widget_count; i++) {
            destroy_flora_widget(&screen->widgets[i]);
        }
        free(screen->widgets);
        screen->widgets = NULL;
        screen->widget_count = 0;
        printf("Log: Screen destroyed successfully\n");
    }
    screen->on_screen_destroy = NULL;
    screen->on_screen_create = NULL;
}

void update_screen(FloraScreen *screen, FloraApplicationState *state) {
    if (!screen) {
        fprintf(stderr, "Error: Screen is not initialized\n");
        return;
    }

    for (int i = 0; i < screen->widget_count; i++) {
        FloraWidget *widget = &screen->widgets[i];
        if (widget && widget->callbacks.update && widget->is_visible) {
            widget->callbacks.update(widget, state);
        }
    }

    while (!is_event_queue_empty(&state->event_queue)) {
        FloraEvent *event;
        if (dequeue_event(&state->event_queue, &event)) {
            switch (event->type) {
                case FLORA_MOUSE_MOVE: {
                    destroy_event(event);
                    break;
                }
                case FLORA_MOUSE_DOWN: {
                    for (int i = 0; i < screen->widget_count; i++) {
                        FloraWidget *widget = &screen->widgets[i];
                        if (widget->is_visible && widget->callbacks.on_mouse_down
                            && widget_contains_point(widget, event->as.mouse_button.x, event->as.mouse_button.y)) {
                            widget->callbacks.on_mouse_down(widget, state);
                            // TODO: How should event propagation be handled
                            break; // Stop after the first widget handles the event
                        }
                    }
                    destroy_event(event);
                    break;
                }
                case FLORA_MOUSE_UP: {
                    destroy_event(event);
                    break;
                }
                case FLORA_KEY_DOWN: {
                    switch (event->as.keyboard.key) {
                        case SDLK_ESCAPE: {
                            state->running = !state->running;
                            break;
                        }
                        default: {
                            printf("Log: Key Pressed: %s\n", SDL_GetKeyName(event->as.keyboard.key));
                            break;
                        }
                    }
                    destroy_event(event);
                    break;
                }
                case FLORA_KEY_UP: {
                    destroy_event(event);
                    break;
                }
                case FLORA_QUIT: {
                    state->running = false;
                    destroy_event(event);
                    break;
                }
                case FLORA_UNHANDLED: {
                    printf("Log: Unhandled Event: %d\n", event->type);
                    destroy_event(event);
                    break;
                }
                default: {
                    printf("Log: Unsupported event type: %d\n", event->type);
                    destroy_event(event);
                    break;
                }
            }
        } else {
            fprintf(stderr, "Error: Failed to dequeue event.\n");
        }
    }
}

void render_screen(FloraScreen *screen, FloraApplicationState *state) {
    if (!screen || !state) {
        fprintf(stderr, "Error: Invalid screen or state.\n");
        return;
    }

    for (int i = 0; i < screen->widget_count; i++) {
        FloraWidget *widget = &screen->widgets[i];
        if (widget->is_visible && widget->callbacks.render) {
            widget->callbacks.render(widget, state);
        }
    }
}

int add_widget_to_screen(FloraScreen *screen, FloraWidget widget) {
    if (!screen) {
        return false;
    }
    if (screen->widget_count + 1 > screen->widget_capacity) {
        int new_capacity = screen->widget_capacity * 2;
        FloraWidget *new_widgets = realloc(screen->widgets, new_capacity * sizeof(FloraWidget));
        if (!new_widgets) {
            fprintf(stderr, "Error: Failed to allocate memory for new widgets.\n");
            return false;
        }
        screen->widgets = new_widgets;
        screen->widget_capacity = new_capacity;
    }
    screen->widgets[screen->widget_count++] = widget;
    printf("Log: Widget added to screen. Total widgets: %d\n", screen->widget_count);
    return screen->widget_count - 1;
}


void base_screen_create(FloraApplicationState *state, FloraScreen *screen) {
    FloraWidget *baseWidget = create_box_widget(
        state,
        NULL,
        (FloraWidgetStyle){
            .inner_colour = FLORA_SLATE_500,
            .border_colour = FLORA_WHITE,
            .padding = (FloraPadding){
                .left = 20.0f,
                .right = 20.0f,
                .top = 10.0f,
                .bottom = 10.0f
            },
            .gap = (FloraGap){.x = 25.0f, .y = 0.0f},
            .layout_direction = LEFT_TO_RIGHT,
            .sizing = (FloraSizing){
                .width = FLORA_WIDTH_FIXED(460),
                .height = FLORA_HEIGHT_FIT(50)
            },
            .position = (FloraPosition){.x = 100, .y = 100}
        },
        (FloraWidgetCallbacks){
            .update = base_box_widget_update,
            .render = base_box_widget_render,
            .on_mouse_down = base_box_widget_on_mouse_down,
            .on_destroy = NULL
        },
        true
    );

    FloraWidget *child1 = create_box_widget(
        state,
        baseWidget,
        (FloraWidgetStyle){
            .inner_colour = FLORA_INDIGO_500,
            .border_colour = FLORA_WHITE,
            .padding = (FloraPadding){0, 0, 0, 0},
            .gap = (FloraGap){0, 0},
            .layout_direction = LEFT_TO_RIGHT,
            .sizing = (FloraSizing){
                .width = FLORA_WIDTH_GROW(125),
                .height = FLORA_HEIGHT_GROW(50)
            },
            .position = (FloraPosition){.x = 50, .y = 50}
        },
        (FloraWidgetCallbacks){
            .update = base_box_widget_update,
            .render = base_box_widget_render,
            .on_mouse_down = base_box_widget_on_mouse_down,
            .on_destroy = NULL
        },
        true
    );

    FloraWidget *child2 = create_box_widget(
        state,
        baseWidget,
        (FloraWidgetStyle){
            .inner_colour = FLORA_EMERALD_500,
            .border_colour = FLORA_WHITE,
            .padding = (FloraPadding){
                .left = 10.0f,
                .right = 10.0f,
                .top = 5.0f,
                .bottom = 5.0f
            },
            .gap = (FloraGap){.x = 0.0f, .y = 20.0f},
            .layout_direction = TOP_TO_BOTTOM,
            .sizing = (FloraSizing){
                .width = FLORA_WIDTH_GROW(100),
                .height = FLORA_HEIGHT_FIT(100)
            },
            .position = (FloraPosition){.x = 50, .y = 50}
        },
        (FloraWidgetCallbacks){
            .update = base_box_widget_update,
            .render = base_box_widget_render,
            .on_mouse_down = base_box_widget_on_mouse_down,
            .on_destroy = NULL
        },
        true
    );

    FloraWidget *child3 = create_box_widget(
        state,
        child2,
        (FloraWidgetStyle){
            .inner_colour = FLORA_CYAN_500,
            .border_colour = FLORA_WHITE,
            .padding = (FloraPadding){0, 0, 0, 0},
            .gap = (FloraGap){0, 0},
            .layout_direction = LEFT_TO_RIGHT,
            .sizing = (FloraSizing){
                .width = FLORA_WIDTH_GROW(50),
                .height = FLORA_HEIGHT_GROW(50)
            },
            .position = (FloraPosition){.x = 50, .y = 50}
        },
        (FloraWidgetCallbacks){
            .update = base_box_widget_update,
            .render = base_box_widget_render,
            .on_mouse_down = base_box_widget_on_mouse_down,
            .on_destroy = NULL
        },
        true
    );

    FloraWidget *child4 = create_box_widget(
        state,
        child2,
        (FloraWidgetStyle){
            .inner_colour = FLORA_AMBER_500,
            .border_colour = FLORA_WHITE,
            .padding = (FloraPadding){0, 0, 0, 0},
            .gap = (FloraGap){0, 0},
            .layout_direction = LEFT_TO_RIGHT,
            .sizing = (FloraSizing){
                .width = FLORA_WIDTH_GROW(90),
                .height = FLORA_HEIGHT_GROW(60)
            },
            .position = (FloraPosition){.x = 50, .y = 50}
        },
        (FloraWidgetCallbacks){
            .update = base_box_widget_update,
            .render = base_box_widget_render,
            .on_mouse_down = base_box_widget_on_mouse_down,
            .on_destroy = NULL
        },
        true
    );

    add_child_widget(baseWidget, child1);
    add_child_widget(baseWidget, child2);
    add_child_widget(child2, child3);
    add_child_widget(child2, child4);
}


bool init_flora_screen(FloraScreen *screen, FloraApplicationState *state) {
    screen->on_screen_create = base_screen_create;
    screen->on_screen_destroy = base_screen_destroy;
    screen->widget_count = 0;
    screen->widget_capacity = INITIAL_WIDGET_CAPACITY;
    screen->widgets =
            (FloraWidget *) malloc(screen->widget_capacity * sizeof(FloraWidget));
    if (!screen->widgets) {
        printf("Log: Failed to allocate memory for scene widgets. Aborting!\n");
        fprintf(stderr, "Error: Failed to allocate memory for scene widgets.\n");
        return false;
    }
    return true;
}
