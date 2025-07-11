#include "flora_widget.h"
#include "app_state.h"
#include <stdio.h>

void calculateWidgetWidthHeight(FloraWidget* widget) {
	if (!widget || !widget->isVisible || widget->childCount == 0) {
		return;
	}

	float totalWidgetWidth = 0.0f;
	float totalWidgetHeight = 0.0f;
	int visibleChildCount = 0;

	if (widget->layoutDirection == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			calculateWidgetWidthHeight(child);
			if (child->isVisible) {
				totalWidgetHeight = child->sizing.height.value > totalWidgetHeight ? child->sizing.height.value : totalWidgetHeight;
				totalWidgetWidth += child->sizing.width.value;
				visibleChildCount++;
			}
		}
		if (visibleChildCount > 0) {
			totalWidgetWidth += (visibleChildCount - 1) * widget->style.gap.x;
		}
	}
	else if (widget->layoutDirection == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			calculateWidgetWidthHeight(child);
			if (child->isVisible) {
				totalWidgetHeight += child->sizing.height.value;
				totalWidgetWidth = child->sizing.width.value > totalWidgetWidth ? child->sizing.width.value : totalWidgetWidth;
				visibleChildCount++;
			}
		}
		if (visibleChildCount > 0) {
			totalWidgetHeight += (visibleChildCount - 1) * widget->style.gap.y;
		}
	}

	if (widget->sizing.width.type == FIT) {
		widget->sizing.width.value = totalWidgetWidth + widget->style.padding.left + widget->style.padding.right;
	}
	if (widget->sizing.height.type == FIT) {
		widget->sizing.height.value = totalWidgetHeight + widget->style.padding.top + widget->style.padding.bottom;
	}
}

void calculateChildrenPositions(FloraWidget* widget) {
	if (!widget || !widget->isVisible || widget->childCount == 0) {
		return;
	}

	float offsetX = widget->style.padding.left;
	float offsetY = widget->style.padding.top;
	if (widget->layoutDirection == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			if (!child->isVisible) {
				continue;
			}
			child->position.x = widget->position.x + offsetX;
			child->position.y = widget->position.y + offsetY;
			offsetX += child->sizing.width.value + widget->style.gap.x;

			calculateChildrenPositions(child);
		}
	}
	else if (widget->layoutDirection == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			if (!child->isVisible) {
				continue;
			}
			child->position.x = widget->position.x + offsetX;
			child->position.y = widget->position.y + offsetY;
			offsetY += child->sizing.height.value + widget->style.gap.y;
			calculateChildrenPositions(child);
		}
	}
}


void calculateGrowSizingLeftToRight(FloraWidget* parent) {
	float remainingWidth = parent->sizing.width.value;
	remainingWidth -= parent->style.padding.left + parent->style.padding.right;
	int visibleChildCount = 0;

	for (int i = 0; i < parent->childCount; i++) {
		FloraWidget* child = parent->children[i];
		visibleChildCount += child->isVisible ? 1 : 0;
	}

	if (visibleChildCount > 0) {
		remainingWidth -= parent->style.gap.x * (visibleChildCount - 1);
	}

	if (parent->layoutDirection == LEFT_TO_RIGHT) {
		for (int i = 0; i < parent->childCount; i++) {
			FloraWidget* child = parent->children[i];
			if (child->isVisible && child->sizing.width.type != GROW) {
				remainingWidth -= child->sizing.width.value;
			}
		}
	}
	int growCount = 0;
	for (int i = 0; i < parent->childCount; i++) {
		FloraWidget* child = parent->children[i];
		if (child->sizing.width.type == GROW) {
			growCount++;
		}
	}

	if (growCount > 0) {
		float growWidth = parent->layoutDirection == LEFT_TO_RIGHT ? remainingWidth / growCount: remainingWidth;
		for (int i = 0; i < parent->childCount; i++) {
			FloraWidget* child = parent->children[i];
			if (child->isVisible && child->sizing.width.type == GROW) {
				child->sizing.width.value = growWidth;
			}
		}
	}
}

void calculateGrowSizingTopToBottom(FloraWidget* parent) {
	float remainingHeight = parent->sizing.height.value;
	remainingHeight -= parent->style.padding.top + parent->style.padding.bottom;
	int visibleChildCount = 0;

	for (int i = 0; i < parent->childCount; i++) {
		FloraWidget* child = parent->children[i];
		visibleChildCount += child->isVisible ? 1 : 0;
	}
	if (visibleChildCount > 0) {
		remainingHeight -= parent->style.gap.y * (visibleChildCount - 1);
	}

	if (parent->layoutDirection == TOP_TO_BOTTOM) {
		for (int i = 0; i < parent->childCount; i++) {
			FloraWidget* child = parent->children[i];
			if (child->isVisible && child->sizing.height.type != GROW) {
				remainingHeight -= child->sizing.height.value;
			}
		}
	}

	int growCount = 0;
	for (int i = 0; i < parent->childCount; i++) {
		FloraWidget* child = parent->children[i];
		if (child->sizing.height.type == GROW) {
			growCount++;
		}
	}

	if (growCount > 0) {
		float growHeight = parent->layoutDirection == TOP_TO_BOTTOM ? remainingHeight / growCount : remainingHeight;
		for (int i = 0; i < parent->childCount; i++) {
			FloraWidget* child = parent->children[i];
			if (child->isVisible && child->sizing.height.type == GROW) {
				child->sizing.height.value = growHeight;
			}
		}
	}
}

void calculateGrowSizing(FloraWidget* parent) {
	calculateGrowSizingLeftToRight(parent);
	calculateGrowSizingTopToBottom(parent);
}

void renderWidget(FloraWidget* widget, ApplicationState* state) {
	if (!widget || !widget->isVisible) {
		return;
	}

	FloraWidgetStyle style = widget->style;
	FloraPosition position = widget->position;

	SDL_SetRenderDrawColor(state->mainRenderer, style.colour.r, style.colour.g, style.colour.b, style.colour.a);
	const SDL_FRect rect = { position.x, position.y, widget->sizing.width.value, widget->sizing.height.value };
	// Draw the rectangle
	SDL_RenderFillRect(state->mainRenderer, &rect);
	// Draw the border
	SDL_SetRenderDrawColor(state->mainRenderer, style.borderColour.r, style.borderColour.g, style.borderColour.b, style.borderColour.a);
	SDL_RenderRect(state->mainRenderer, &rect);

	// Render children
	for (int i = 0; i < widget->childCount; i++) {
		FloraWidget* child = widget->children[i];
		if (child->isVisible) {
			renderWidget(child, state);
		}
	}
}

/*
* 1. The position of a widget is relative to the position of its parent.
* 2. The size of an element is dependent on the size of its children.
*/

void baseWidgetRender(FloraWidget* widget, ApplicationState* state) {
	if (!widget || !widget->isVisible || widget->childCount == 0) {
		return;
	}


	calculateWidgetWidthHeight(widget);
	calculateGrowSizing(widget);
	calculateChildrenPositions(widget);
	renderWidget(widget, state);
}

void baseWidgetUpdate(FloraWidget* widget, ApplicationState* state) {

}

void baseWidgetOnClick(FloraWidget* widget, ApplicationState* state,
					   SDL_MouseButtonEvent* event) {

}

bool widgetContainsPoint(FloraWidget* widget, float x, float y) {
	if (!widget || !widget->isVisible)
		return false;
	return (x >= widget->position.x && x <= widget->position.x + widget->sizing.width.value &&
			y >= widget->position.y && y <= widget->position.y + widget->sizing.height.value);
}
