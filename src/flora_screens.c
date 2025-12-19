#include <stdio.h>
#include <stdlib.h>

#include "flora_screens.h"
#include "flora_constants.h"

void base_screen_destroy(FloraApplicationState* state, FloraScreen* screen) {
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

void update_screen(FloraScreen* screen, FloraApplicationState* state) {
    if (!screen) {
        fprintf(stderr, "Error: Screen is not initialized\n");
        return;
    }

    for (int i = 0; i < screen->widget_count; i++) {
        FloraWidget* widget = &screen->widgets[i];
        if (widget && widget->update && widget->is_visible) {
            widget->update(widget, state);
        }
    }

    while (!is_event_queue_empty(&state->event_queue)) {
        FloraEvent* event;
        if (dequeue_event(&state->event_queue, &event)) {
            switch (event->type) {
                case FLORA_MOUSE_MOVE: {
                    destroy_event(event);
                    break;
                }
                case FLORA_MOUSE_DOWN:{
                    for (int i = 0; i < screen->widget_count; i++) {
                        FloraWidget* widget = &screen->widgets[i];
                        if (widget->is_visible && widget->on_mouse_down
                            && widget_contains_point(widget, event->as.mouse_button.x, event->as.mouse_button.y)) {
                                widget->on_mouse_down(widget, state);
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
                case FLORA_KEY_DOWN:{
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
        }else{
            fprintf(stderr, "Error: Failed to dequeue event.\n");
        }
    }
}

void render_screen(FloraScreen* screen, FloraApplicationState* state) {
    if (!screen || !state) {
        fprintf(stderr, "Error: Invalid screen or state.\n");
        return;
    }

    for (int i = 0; i < screen->widget_count; i++) {
        FloraWidget* widget = &screen->widgets[i];
        if (widget->is_visible && widget->render) {
            widget->render(widget, state);
        }
    }
}

int add_widget_to_screen(FloraScreen* screen, FloraWidget widget) {
    if (!screen) {
        return false;
    }
    if (screen->widget_count + 1 > screen->widget_capacity) {
        int new_capacity = screen->widget_capacity * 2;
        FloraWidget* new_widgets = realloc(screen->widgets, new_capacity * sizeof(FloraWidget));
        if (!new_widgets) {
            fprintf(stderr, "Error: Failed to allocate memory for new widgets.\n");
            return false;
        }
        screen->widgets = new_widgets;
        screen->widget_capacity = new_capacity;
    }
    screen->widgets[screen->widget_count++] = widget;
    printf("Log: Widget added to screen. Total widgets: %d\n", screen->widget_count);
    return screen->widget_count-1;
}

bool add_child_to_widget(FloraWidget* parent, FloraWidget* child) {
	if (!parent || !child) {
		return false;
	}

	if (parent->children == NULL) {
		parent->children = (FloraWidget**)malloc(INITIAL_CHILD_WIDGET_CAPACITY * sizeof(FloraWidget*));
		if (!parent->children) {
			printf("Log: Failed to allocate memory for children. Aborting!\n");
			fprintf(stderr, "Error: Failed to allocate memory for children.\n");
			exit(1);
		}
		parent->child_capacity = INITIAL_CHILD_WIDGET_CAPACITY;
		printf("Log: Initialized child array with capacity: %d\n", parent->child_capacity);
	}

	if (parent->child_count + 1 > parent->child_capacity) {
		int newCapacity = parent->child_capacity * GROWTH_FACTOR;
		FloraWidget** newChildren = (FloraWidget**)realloc(
			parent->children, newCapacity * sizeof(FloraWidget*));
		if (!newChildren) {
			printf("Log: Failed to allocate memory for new children. Aborting!\n");
			fprintf(stderr, "Error: Failed to allocate memory for new children.\n");
			exit(1);
		}
		parent->children = newChildren;
		parent->child_capacity = newCapacity;
		printf("Log: Resized child array to new capacity: %d\n", newCapacity);
	}

	parent->children[parent->child_count++] = child;
	child->parent = parent;
	printf("Log: Child widget added to parent. Total children: %d\n", parent->child_count);
	return true;
}

void base_screen_create(FloraApplicationState* state, FloraScreen* screen) {
	FloraWidget baseWidget = {
				.id = 1,
				.position = (FloraPosition){.x = 100, .y = 100},
				.sizing = (FloraSizing){
					.width = FLORA_WIDTH_FIXED(460),
					.height = FLORA_HEIGHT_FIT(50)
				},
				.is_visible = true,
				.children = NULL,
				.parent = NULL,
				.child_count = 0,
				.update = base_widget_update,
				.render = base_widget_render,
				.on_mouse_down = base_widget_on_mouse_down,
				.style = (FloraWidgetStyle){
					.inner_colour = FLORA_SLATE_500,
					.border_colour = FLORA_WHITE,
					.padding = (FloraPadding){
						.left = 20.0f,
						.right = 20.0f,
						.top = 10.0f,
						.bottom = 10.0f
					},
		.gap = (FloraGap){.x = 25 }
				},
		.layout_direction = LEFT_TO_RIGHT
	};
	FloraWidget child1 = {
				.id = 2,
				.sizing = (FloraSizing){
					.width = FLORA_WIDTH_GROW(125),
					.height = FLORA_HEIGHT_GROW(50)
				},
				.position = (FloraPosition){.x = 50, .y = 50},
				.is_visible = true,
				.children = NULL,
				.child_count = 0,
				.parent = NULL,
				.update = base_widget_update,
				.render = base_widget_render,
				.on_mouse_down = base_widget_on_mouse_down,
				.style = (FloraWidgetStyle){
					.inner_colour = FLORA_INDIGO_500,
					.border_colour = FLORA_WHITE
				}
	};

	FloraWidget child2 = {
				.id = 3,
				.sizing = (FloraSizing){
					.width = FLORA_WIDTH_GROW(100),
					.height = FLORA_HEIGHT_FIT(100)
				},
				.position = (FloraPosition){.x = 50, .y = 50},
				.is_visible = true,
				.children = NULL,
				.child_count = 0,
				.parent = NULL,
				.update = base_widget_update,
				.render = base_widget_render,
				.on_mouse_down = base_widget_on_mouse_down,
				.layout_direction = TOP_TO_BOTTOM,
				.style = (FloraWidgetStyle){
					.inner_colour = FLORA_EMERALD_500,
					.border_colour = FLORA_WHITE,
					.padding = (FloraPadding){
						.left = 10.0f,
						.right = 10.0f,
						.top = 5.0f,
						.bottom = 5.0f
					},
					.gap = (FloraGap){.y = 20 }
				}
	};

	FloraWidget child3 = {
				.id = 4,
				.position = (FloraPosition){.x = 50, .y = 50},
				.sizing = (FloraSizing){
					.width = FLORA_WIDTH_GROW(50),
					.height = FLORA_HEIGHT_GROW(50)
				},
				.is_visible = true,
				.children = NULL,
				.child_count = 0,
				.parent = NULL,
				.update = base_widget_update,
				.render = base_widget_render,
				.on_mouse_down = base_widget_on_mouse_down,
				.style = (FloraWidgetStyle){
					.inner_colour = FLORA_CYAN_500 ,
					.border_colour = FLORA_WHITE
				}
	};

	FloraWidget child4 = {
				.id = 5,
				.position = (FloraPosition){.x = 50, .y = 50},
				.sizing = (FloraSizing){
					.width = FLORA_WIDTH_GROW(90),
					.height = FLORA_HEIGHT_GROW(60)
				},
				.is_visible = true,
				.children = NULL,
				.child_count = 0,
				.parent = NULL,
				.update = base_widget_update,
				.render = base_widget_render,
				.on_mouse_down = base_widget_on_mouse_down,
				.style = (FloraWidgetStyle){
					.inner_colour = FLORA_AMBER_500,
					.border_colour = FLORA_WHITE
				}
	};

	int baseIndex = add_widget_to_screen(screen, baseWidget);

	if (baseIndex == -1) {
		printf("Log: Failed to add base widget to scene.\n");
		fprintf(stderr, "Error: Failed to add base widget to scene.\n");
		exit(1);
	}
	int child1Index = add_widget_to_screen(screen, child1);

	if (child1Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	int child2Index = add_widget_to_screen(screen, child2);

	if (child2Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	int child3Index = add_widget_to_screen(screen, child3);
	if (child3Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	int child4Index = add_widget_to_screen(screen, child4);
	if (child4Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	add_child_to_widget(&screen->widgets[baseIndex], &screen->widgets[child1Index]);
	add_child_to_widget(&screen->widgets[baseIndex], &screen->widgets[child2Index]);
	add_child_to_widget(&screen->widgets[child2Index], &screen->widgets[child3Index]);
	add_child_to_widget(&screen->widgets[child2Index], &screen->widgets[child4Index]);
}


bool init_flora_screen(FloraScreen* screen, FloraApplicationState* state) {
	screen->on_screen_create = base_screen_create;
	screen->on_screen_destroy = base_screen_destroy;
	screen->widget_count = 0;
	screen->widget_capacity = INITIAL_WIDGET_CAPACITY;
	screen->widgets =
	(FloraWidget*)malloc(screen->widget_capacity * sizeof(FloraWidget));
	if (!screen->widgets) {
		printf("Log: Failed to allocate memory for scene widgets. Aborting!\n");
		fprintf(stderr, "Error: Failed to allocate memory for scene widgets.\n");
		return false;
	}
	return true;
}
