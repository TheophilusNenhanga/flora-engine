#include "flora_widget.h"
#include "app_state.h"
#include <stdio.h>

void calculateWidgetWidthHeight(FloraWidget* widget) {
	if (!widget || !widget->isVisible || widget->childCount == 0) {
		return;
	}

	float totalWidgetWidth = 0.0f;
	float totalWidgetHeight = 0.0f;

	if (widget->layout == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			calculateWidgetWidthHeight(child);
			if (child->isVisible) {
				totalWidgetHeight = child->height > totalWidgetHeight ? child->height : totalWidgetHeight;
				totalWidgetWidth += child->width;
			}
		}
		totalWidgetWidth += (widget->childCount - 1) * widget->style.gap.x;
	} else if (widget->layout == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			calculateWidgetWidthHeight(child);
			if (child->isVisible) {
				totalWidgetHeight += child->height;
				totalWidgetWidth = child->width > totalWidgetWidth ? child->width : totalWidgetWidth;
			}
		}
		totalWidgetHeight += (widget->childCount - 1) * widget->style.gap.y;
	}
	widget->width = totalWidgetWidth += widget->style.padding.left + widget->style.padding.right;
	widget->height = totalWidgetHeight += widget->style.padding.top + widget->style.padding.bottom;
}

void calculateChildrenPositions(FloraWidget* widget) {
	if (!widget || !widget->isVisible || widget->childCount == 0) {
		return;
	}

	float offsetX = widget->style.padding.left;
	float offsetY = widget->style.padding.top;
	if (widget->layout == LEFT_TO_RIGHT) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			if (!child->isVisible) {
				continue; 
			}
			child->posX = widget->posX + offsetX;
			child->posY = widget->posY + offsetY;
			offsetX += child->width + widget->style.gap.x;

			calculateChildrenPositions(child);
		}
	} else if (widget->layout == TOP_TO_BOTTOM) {
		for (int i = 0; i < widget->childCount; i++) {
			FloraWidget* child = widget->children[i];
			if (!child->isVisible) {
				continue; 
			}
			child->posX = widget->posX + offsetX;
			child->posY = widget->posY + offsetY;
			offsetY += child->height + widget->style.gap.y;
			calculateChildrenPositions(child);
		}
	}
}

void renderWidget(FloraWidget* widget, ApplicationState* state) {
	if (!widget || !widget->isVisible) {
		return;
	}
	SDL_SetRenderDrawColor(state->mainRenderer, widget->style.colour.r,
						   widget->style.colour.g, widget->style.colour.b, widget->style.colour.a);
	const SDL_FRect rect = { widget->posX, widget->posY, widget->width,
							widget->height };
	// Draw the rectangle
	SDL_RenderFillRect(state->mainRenderer, &rect);
	// Draw the border
	SDL_SetRenderDrawColor(state->mainRenderer, widget->style.borderColour.r,
						   widget->style.borderColour.g, widget->style.borderColour.b,
						   widget->style.borderColour.a);
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
	calculateChildrenPositions(widget);
	renderWidget(widget, state);
}

void baseWidgetUpdate(FloraWidget* widget, ApplicationState* state) {
	 //widget->posX += 30 * (float)state->deltaTime;
	 //if (widget->posX > state->windowWidth) {
	 //	widget->posX = 0;
	 //}
	 //widget->posY += 30 * (float)state->deltaTime;
	 //if (widget->posY > state->windowHeight) {
	 //	widget->posY = 0;
	 //}
}

void baseWidgetOnClick(FloraWidget* widget, ApplicationState* state,
					   SDL_MouseButtonEvent* event) {
	if (event->button == SDL_BUTTON_LEFT) {
		widget->posX = widget->posX - 10;
		widget->posY = widget->posY - 10;
	}
	else if (event->button == SDL_BUTTON_RIGHT) {
		widget->posX = widget->posX + 10;
		widget->posY = widget->posY + 10;
	}
	
}

bool widgetContainsPoint(FloraWidget* widget, float x, float y) {
	if (!widget || !widget->isVisible)
		return false;
	return (x >= widget->posX && x <= widget->posX + widget->width &&
			y >= widget->posY && y <= widget->posY + widget->height);
}
