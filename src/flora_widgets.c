#include "flora_widgets.h"
#include "flora_apps.h"
#include <stdlib.h>


void calc_widget_dimensions(FloraWidget* widget) {
	if (!widget || !widget->is_visible || widget->child_count == 0) {
		return;
	}

	float totalWidgetWidth = 0.0f;
	float totalWidgetHeight = 0.0f;
	int visible_child_count = 0;

	if (widget->layout_direction == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->child_count; i++) {
			FloraWidget* child = widget->children[i];
			calc_widget_dimensions(child);
			if (child->is_visible) {
				totalWidgetHeight = child->sizing.height.value > totalWidgetHeight ? child->sizing.height.value : totalWidgetHeight;
				totalWidgetWidth += child->sizing.width.value;
				visible_child_count++;
			}
		}
		if (visible_child_count > 0) {
		totalWidgetWidth += (visible_child_count - 1) * widget->style.gap.x;
		}
	}
	else if (widget->layout_direction == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->child_count; i++) {
			FloraWidget* child = widget->children[i];
			calc_widget_dimensions(child);
			if (child->is_visible) {
				totalWidgetHeight += child->sizing.height.value;
				totalWidgetWidth = child->sizing.width.value > totalWidgetWidth ? child->sizing.width.value : totalWidgetWidth;
				visible_child_count++;
			}
		}
		if (visible_child_count > 0) {
		totalWidgetHeight += (visible_child_count - 1) * widget->style.gap.y;
		}
	}

	if (widget->sizing.width.type == FIT) {
		widget->sizing.width.value = totalWidgetWidth + widget->style.padding.left + widget->style.padding.right;
	}
	if (widget->sizing.height.type == FIT) {
		widget->sizing.height.value = totalWidgetHeight + widget->style.padding.top + widget->style.padding.bottom;
	}
}

void calc_child_positions(FloraWidget* widget) {
	if (!widget || !widget->is_visible || widget->child_count == 0) {
		return;
	}

	float offsetX = widget->style.padding.left;
	float offsetY = widget->style.padding.top;
	if (widget->layout_direction == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->child_count; i++) {
			FloraWidget* child = widget->children[i];
			if (!child->is_visible) {
				continue;
			}
			child->position.x = widget->position.x + offsetX;
			child->position.y = widget->position.y + offsetY;
			offsetX += child->sizing.width.value + widget->style.gap.x;

			calc_child_positions(child);
		}
	}
	else if (widget->layout_direction == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->child_count; i++) {
			FloraWidget* child = widget->children[i];
			if (!child->is_visible) {
				continue;
			}
			child->position.x = widget->position.x + offsetX;
			child->position.y = widget->position.y + offsetY;
			offsetY += child->sizing.height.value + widget->style.gap.y;
			calc_child_positions(child);
		}
	}
}


void calc_grow_sizing_left_to_right(const FloraWidget* widget) {
	float remainingWidth = widget->sizing.width.value;
	remainingWidth -= widget->style.padding.left + widget->style.padding.right;
	int visible_child_count = 0;

	for (int i = 0; i < widget->child_count; i++) {
		const FloraWidget* child = widget->children[i];
		visible_child_count += child->is_visible ? 1 : 0;
	}

	if (visible_child_count > 0) {
		remainingWidth -= widget->style.gap.x * (visible_child_count - 1);
	}

	if (widget->layout_direction == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->child_count; i++) {
			const FloraWidget* child = widget->children[i];
			if (child->is_visible && child->sizing.width.type != GROW) {
				remainingWidth -= child->sizing.width.value;
			}
		}
	}
	int growCount = 0;
	for (int i = 0; i < widget->child_count; i++) {
		const FloraWidget* child = widget->children[i];
		if (child->sizing.width.type == GROW) {
			growCount++;
		}
	}

	if (growCount > 0) {
		const float growWidth = widget->layout_direction == LEFT_TO_RIGHT ? remainingWidth / growCount: remainingWidth;
		for (int i = 0; i < widget->child_count; i++) {
			FloraWidget* child = widget->children[i];
			if (child->is_visible && child->sizing.width.type == GROW) {
				child->sizing.width.value = growWidth;
			}
		}
	}
}

void calc_grow_sizing_top_to_bottom(const FloraWidget* widget) {
	float remainingHeight = widget->sizing.height.value;
	remainingHeight -= widget->style.padding.top + widget->style.padding.bottom;
	int visible_child_count = 0;

	for (int i = 0; i < widget->child_count; i++) {
		FloraWidget* child = widget->children[i];
		visible_child_count += child->is_visible ? 1 : 0;
	}
	if (visible_child_count > 0) {
		remainingHeight -= widget->style.gap.y * (visible_child_count - 1);
	}

	if (widget->layout_direction == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->child_count; i++) {
			const FloraWidget* child = widget->children[i];
			if (child->is_visible && child->sizing.height.type != GROW) {
				remainingHeight -= child->sizing.height.value;
			}
		}
	}

	int growCount = 0;
	for (int i = 0; i < widget->child_count; i++) {
		const FloraWidget* child = widget->children[i];
		if (child->sizing.height.type == GROW) {
			growCount++;
		}
	}

	if (growCount > 0) {
		const float growHeight = widget->layout_direction == TOP_TO_BOTTOM ? remainingHeight / growCount : remainingHeight;
		for (int i = 0; i < widget->child_count; i++) {
			FloraWidget* child = widget->children[i];
			if (child->is_visible && child->sizing.height.type == GROW) {
				child->sizing.height.value = growHeight;
			}
		}
	}
}

void calc_grow_sizing(const FloraWidget* widget) {
    calc_grow_sizing_left_to_right(widget);
    calc_grow_sizing_top_to_bottom(widget);
}

void render_widget(FloraWidget* widget, FloraApplicationState* state) {
    if (!widget || !widget->is_visible) {
        return;
    }

    FloraWidgetStyle style = widget->style;
    FloraPosition position = widget->position;

    SDL_SetRenderDrawColor(state->renderer, style.inner_colour.r, style.inner_colour.g, style.inner_colour.b, style.inner_colour.a);
    const SDL_FRect rect = {position.x, position.y, widget->sizing.width.value, widget->sizing.height.value};
    SDL_RenderFillRect(state->renderer, &rect);
    SDL_SetRenderDrawColor(state->renderer, style.border_colour.r, style.border_colour.g, style.border_colour.b, style.border_colour.a);
    SDL_RenderRect(state->renderer, &rect);

    for (int i = 0; i < widget->child_count; i++) {
        FloraWidget* child = widget->children[i];
        if (child && child->is_visible) {
            render_widget(child, state);
        }
    }
}

void base_widget_render(FloraWidget* widget, FloraApplicationState* state) {
    if (!widget || !widget->is_visible || widget->child_count == 0) {
        return;
    }

    calc_widget_dimensions(widget);
    calc_grow_sizing(widget);
    calc_child_positions(widget);
    render_widget(widget, state);
}

void base_widget_update(FloraWidget* widget, FloraApplicationState* state) {
    (void) widget;
    (void) state;
}

void base_widget_on_mouse_down(FloraWidget* widget, FloraApplicationState* state) {
    (void) widget;
    (void) state;
}

bool widget_contains_point(FloraWidget* widget, int x, int y) {
    if (!widget || !widget->is_visible) {
        return false;
    }

    return x >= widget->position.x && x < widget->position.x + widget->sizing.width.value &&
           y >= widget->position.y && y < widget->position.y + widget->sizing.height.value;
}

bool destroy_flora_widget(FloraWidget* widget) {
    if (!widget) {
        return false;
    }

    for (int i = 0; i < widget->child_count; i++) {
        FloraWidget* child = widget->children[i];
        if (child) {
            destroy_flora_widget(child);
        }
    }
    free(widget->children);
    return true;
}
