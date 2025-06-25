#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

#include "flora_constants.h"

void initSceneManager(SceneManager* manager, ApplicationState* state) {
	manager->state = state;
	manager->currentScene = NULL;
}

void setCurrentScene(SceneManager* manager, FloraScene* scene) {
	manager->currentScene = scene;
	scene->onCreate(manager->state, scene);
}

void updateScene(SceneManager* manager) {
	if (manager->currentScene) {
		FloraScene* scene = manager->currentScene;
		for (int i = 0; i < scene->widgetCount; i++) {
			FloraWidget* widget = &scene->widgets[i];
			if (widget->isVisible) {
				widget->update(widget, manager->state);
			}
		}

		while (!isEventQueueEmpty(&manager->state->eventQueue)) {
			FloraEvent* event;
			if (dequeueEventQueue(&manager->state->eventQueue, &event)) {
				switch (event->type) {
					case FLORA_MOUSE_MOVE: {
							cleanupFloraEvent(event);
							break;
						}
					case FLORA_MOUSE_BUTTON_DOWN: {
							for (int i = 0; i < scene->widgetCount; i++) {
								FloraWidget* widget = &scene->widgets[i];
								if (widget->isVisible && widget->onClick &&
									widgetContainsPoint(widget, event->as.mouseButton.x,
														event->as.mouseButton.y)) {
									widget->onClick(widget, manager->state, &event->as.mouseButton);
									break; // Stop after the first widget that handles the click
								}
							}
							cleanupFloraEvent(event);
							break;
						}
					case FLORA_MOUSE_BUTTON_UP: {
							cleanupFloraEvent(event);
							break;
						}
					case FLORA_KEY_DOWN: {
							switch (event->as.key.key) {
								case SDLK_ESCAPE: {
										manager->state->running = false;
										cleanupFloraEvent(event);
										break;
									}
								default: {
										printf("Log: Key down event: %s\n",
											   SDL_GetKeyName(event->as.key.key));
										cleanupFloraEvent(event);
										break;
									}
							}
							break;
						}
					case FLORA_KEY_UP: {
							cleanupFloraEvent(event);
							break;
						}
					case FLORA_QUIT: {
							manager->state->running = false;
							cleanupFloraEvent(event);
							break;
						}
					case FLORA_UNHANDLED: {
							cleanupFloraEvent(event);
							break;
						}
					default: {
							printf("Log: Unsupported event type: %d\n", event->type);
							cleanupFloraEvent(event);
							break;
						}
				}
			}
			else {
				printf("Log: Failed to dequeue event from queue.\n");
			}
		}
	}
}

void renderScene(SceneManager* manager) {
	if (manager->currentScene) {
		FloraScene* scene = manager->currentScene;
		for (int i = 0; i < scene->widgetCount; i++) {
			FloraWidget* widget = &scene->widgets[i];
			if (widget->isVisible) {
				widget->render(widget, manager->state);
			}
		}
	}
}

void addWidgetToScene(FloraScene* scene, FloraWidget widget) {
	if (!scene) {
		return;
	}

	if (scene->widgetCount + 1 >= scene->widgetCapacity) {
		int newCapacity = scene->widgetCapacity * 2;
		FloraWidget* newWidgets = (FloraWidget*)realloc(
			scene->widgets, newCapacity * sizeof(FloraWidget));
		if (!newWidgets) {
			printf("Log: Failed to allocate memory for new widgets. Aborting!\n");
			fprintf(stderr, "Error: Failed to allocate memory for new widgets.\n");
			exit(1);
		}
		scene->widgets = newWidgets;
		scene->widgetCapacity = newCapacity;
		printf("Log: Resized widget array to new capacity: %d\n", newCapacity);
	}

	scene->widgets[scene->widgetCount++] = widget;
	printf("Log: Widget added to scene. Total widgets: %d\n", scene->widgetCount);
}

void baseCreateScene(ApplicationState* state, FloraScene* scene) {
	for (int i = 0; i < 5; i++) {
		addWidgetToScene(scene,
						 (FloraWidget) {
			.posX = 50,
				.posY = 50 + i * 50,
				.width = 50,
				.height = 50,
				.isVisible = true,
				.update = baseWidgetUpdate,
				.render = baseWidgetRender,
				.onClick = baseWidgetOnClick,
				.style = (FloraWidgetStyle){
					.colour = (FloraColour){.r = 25, .g = 132, .b = 191, .a = 255 },
					.borderColour = (FloraColour){.r = 255, .g = 255, .b = 255, .a = 255 }
				}
			});
	}
}

void baseDestroyScene(ApplicationState* state, FloraScene* scene) {
	if (scene->widgets) {
		free(scene->widgets);
		scene->widgets = NULL;
		scene->widgetCount = 0;
		printf("Log: Scene destroyed and memory freed.\n");
	}
	scene->onCreate = NULL;
	scene->onDestroy = NULL;
}

void initFloraScene(FloraScene* scene, ApplicationState* state) {
	scene->onCreate = baseCreateScene;
	scene->onDestroy = baseDestroyScene;
	scene->widgetCount = 0;
	scene->widgetCapacity = INITIAL_WIDGET_CAPACITY;
	scene->widgets =
		(FloraWidget*)malloc(scene->widgetCapacity * sizeof(FloraWidget));
	if (!scene->widgets) {
		printf("Log: Failed to allocate memory for scene widgets. Aborting!\n");
		fprintf(stderr, "Error: Failed to allocate memory for scene widgets.\n");
		exit(1);
	}
}
