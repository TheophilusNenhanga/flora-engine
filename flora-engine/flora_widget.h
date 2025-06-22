#ifndef FLORA_WIDGET_H
#define FLORA_WIDGET_H

#include <stdbool.h>

struct ApplicationState;

typedef struct FloraWidget {
	float posX;
	float posY;
	float width;
	float height;
	bool isVisible;

	void (*update)(struct FloraWidget* widget, struct ApplicationState* state);
	void (*render)(struct FloraWidget* widget, struct ApplicationState* state);
	void (*onClick)(struct FloraWidget* widget, struct ApplicationState* state, int mouseX, int mouseY);
} FloraWidget;

void baseWidgetRender(FloraWidget* widget, struct ApplicationState* state);
void baseWidgetUpdate(FloraWidget* widget, struct ApplicationState* state);

#endif