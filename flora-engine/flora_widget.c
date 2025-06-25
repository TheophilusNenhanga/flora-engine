#include "flora_widget.h"
#include "app_state.h"
#include <stdio.h>

void baseWidgetRender(FloraWidget *widget, ApplicationState *state) {
  SDL_SetRenderDrawColor(state->mainRenderer, widget->style.colour.r,
                         widget->style.colour.g, widget->style.colour.b, widget->style.colour.a);
  const SDL_FRect rect = {widget->posX, widget->posY, widget->width,
                          widget->height};
  // Draw the rectangle
  SDL_RenderFillRect(state->mainRenderer, &rect);
  // Draw the border
  SDL_SetRenderDrawColor(state->mainRenderer, widget->style.borderColour.r,
                         widget->style.borderColour.g, widget->style.borderColour.b,
						 widget->style.borderColour.a);
  SDL_RenderRect(state->mainRenderer, &rect);
}

void baseWidgetUpdate(FloraWidget *widget, ApplicationState *state) {
  widget->posX += 30 * (float)state->deltaTime;
  if (widget->posX > state->windowWidth) {
    widget->posX = 0;
  }
  widget->posY += 30 * (float)state->deltaTime;
  if (widget->posY > state->windowHeight) {
    widget->posY = 0;
  }
}

void baseWidgetOnClick(FloraWidget *widget, ApplicationState *state,
                       SDL_MouseButtonEvent *event) {
  widget->posX = event->x - 10;
  widget->posY = event->y - 10;
}

bool widgetContainsPoint(FloraWidget *widget, float x, float y) {
  if (!widget || !widget->isVisible)
    return false;
  return (x >= widget->posX && x <= widget->posX + widget->width &&
          y >= widget->posY && y <= widget->posY + widget->height);
}
