#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

#include "flora_constants.h"

void cleanupFloraWidget(FloraWidget* widget) {
	if (widget->children) {
		free(widget->children);
		widget->children = NULL;
		widget->childCount = 0;
		widget->childCapacity = 0;
		printf("Log: Widget children cleaned up and memory freed.\n");
	}
	widget->parent = NULL;
	widget->isVisible = false;
}

void baseDestroyScene(ApplicationState* state, FloraScene* scene) {
	if (scene->widgets) {
		for (int i = 0; i < scene->widgetCount; i++) {
			cleanupFloraWidget(&scene->widgets[i]);
		}
		free(scene->widgets);
		scene->widgets = NULL;
		scene->widgetCount = 0;
		printf("Log: Scene destroyed and memory freed.\n");
	}
	scene->onCreate = NULL;
	scene->onDestroy = NULL;
}

void initSceneManager(SceneManager* manager, ApplicationState* state) {
	manager->state = state;
	manager->currentScene = NULL;
}

void cleanupSceneManager(SceneManager* manager) {
	if (manager->currentScene) {
		FloraScene* scene = manager->currentScene;
		if (scene->onDestroy) {
			scene->onDestroy(manager->state, scene);
		}
		free(scene->widgets);
		manager->currentScene = NULL;
	}
	printf("Log: Scene manager cleaned up successfully.\n");
}

void setCurrentScene(SceneManager* manager, FloraScene* scene) {
	manager->currentScene = scene;
	scene->onCreate(manager->state, scene);
	scene->onDestroy = baseDestroyScene;
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

int addWidgetToScene(FloraScene* scene, FloraWidget widget) {
	if (!scene) {
		return -1;
	}

	if (scene->widgetCount + 1 > scene->widgetCapacity) {
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
	return scene->widgetCount - 1;
}

bool addChildToWidget(FloraWidget* parent, FloraWidget* child) {
	if (!parent || !child) {
		return false;
	}

	if (parent->children == NULL) {
		parent->children = (FloraWidget*)malloc(INITIAL_CHILD_WIDGET_CAPACITY * sizeof(FloraWidget*));
		if (!parent->children) {
			printf("Log: Failed to allocate memory for children. Aborting!\n");
			fprintf(stderr, "Error: Failed to allocate memory for children.\n");
			exit(1);
		}
		parent->childCapacity = INITIAL_CHILD_WIDGET_CAPACITY;
		printf("Log: Initialized child array with capacity: %d\n", parent->childCapacity);
	}
	
	if (parent->childCount + 1 > parent->childCapacity) {
		int newCapacity = parent->childCapacity * GROWTH_FACTOR;
		FloraWidget* newChildren = (FloraWidget*)realloc(
			parent->children, newCapacity * sizeof(FloraWidget*));
		if (!newChildren) {
			printf("Log: Failed to allocate memory for new children. Aborting!\n");
			fprintf(stderr, "Error: Failed to allocate memory for new children.\n");
			exit(1);
		}
		parent->children = newChildren;
		parent->childCapacity = newCapacity;
		printf("Log: Resized child array to new capacity: %d\n", newCapacity);
	}

	parent->children[parent->childCount++] = child;
	child->parent = parent;
	printf("Log: Child widget added to parent. Total children: %d\n", parent->childCount);
	return true;
}

void baseCreateScene(ApplicationState* state, FloraScene* scene) {
	FloraWidget baseWidget = {
				.id = 1,
				.posX = 100,
				.posY = 100,
				.width = 50,
				.height = 50,
				.isVisible = true,
				.children = NULL,
				.parent = NULL,
				.childCount = 0,
				.update = baseWidgetUpdate,
				.render = baseWidgetRender,
				.onClick = baseWidgetOnClick,
				.style = (FloraWidgetStyle){
					.colour = (FloraColour){.r = 25, .g = 132, .b = 191, .a = 255 },
					.borderColour = (FloraColour){.r = 255, .g = 255, .b = 255, .a = 255 },
					.padding = (FloraPadding){
						.left = 20.0f,
						.right = 20.0f,
						.top = 10.0f,
						.bottom = 10.0f
					}
				},
		.layout = LEFT_TO_RIGHT
	};
	FloraWidget child1 = { 
				.id = 2,
				.posX = 50,
				.posY = 50,
				.width = 125,
				.height = 25,
				.isVisible = true,
				.children = NULL,
				.childCount = 0,
				.parent = NULL,
				.update = baseWidgetUpdate,
				.render = baseWidgetRender,
				.onClick = baseWidgetOnClick,
				.style = (FloraWidgetStyle){
					.colour = (FloraColour){.r = 255, .g = 0, .b = 0, .a = 255 },
					.borderColour = (FloraColour){.r = 255, .g = 255, .b = 255, .a = 255 }
				}
	};

	FloraWidget child2 = {
				.id = 3,
				.posX = 50,
				.posY = 50,
				.width = 40,
				.height = 60,
				.isVisible = true,
				.children = NULL,
				.childCount = 0,
				.parent = NULL,
				.update = baseWidgetUpdate,
				.render = baseWidgetRender,
				.onClick = baseWidgetOnClick,
				.layout = TOP_TO_BOTTOM,
				.style = (FloraWidgetStyle){
					.colour = (FloraColour){.r = 255, .g = 0, .b = 0, .a = 255 },
					.borderColour = (FloraColour){.r = 255, .g = 255, .b = 255, .a = 255 },
					.padding = (FloraPadding){
						.left = 10.0f,
						.right = 10.0f,
						.top = 5.0f,
						.bottom = 5.0f
					}
				}
	};

	FloraWidget child3 = {
				.id = 4,
				.posX = 50,
				.posY = 50,
				.width = 20,
				.height = 30,
				.isVisible = true,
				.children = NULL,
				.childCount = 0,
				.parent = NULL,
				.update = baseWidgetUpdate,
				.render = baseWidgetRender,
				.onClick = baseWidgetOnClick,
				.style = (FloraWidgetStyle){
					.colour = (FloraColour){.r = 255, .g = 123, .b = 196, .a = 255 },
					.borderColour = (FloraColour){.r = 255, .g = 255, .b = 255, .a = 255 }
				}
	};

	FloraWidget child4 = {
				.id = 5,
				.posX = 50,
				.posY = 50,
				.width = 90,
				.height = 30,
				.isVisible = true,
				.children = NULL,
				.childCount = 0,
				.parent = NULL,
				.update = baseWidgetUpdate,
				.render = baseWidgetRender,
				.onClick = baseWidgetOnClick,
				.style = (FloraWidgetStyle){
					.colour = (FloraColour){.r = 11, .g = 123, .b = 196, .a = 255 },
					.borderColour = (FloraColour){.r = 255, .g = 255, .b = 255, .a = 255 }
				}
	};

	int baseIndex = addWidgetToScene(scene, baseWidget);

	if (baseIndex == -1) {
		printf("Log: Failed to add base widget to scene.\n");
		fprintf(stderr, "Error: Failed to add base widget to scene.\n");
		exit(1);
	}
	int child1Index = addWidgetToScene(scene, child1);

	if (child1Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	int child2Index = addWidgetToScene(scene, child2);

	if (child2Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	int child3Index = addWidgetToScene(scene, child3);
	if (child3Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	int child4Index = addWidgetToScene(scene, child4);
	if (child4Index == -1) {
		printf("Log: Failed to add child widget to scene.\n");
		fprintf(stderr, "Error: Failed to add child widget to scene.\n");
		exit(1);
	}

	addChildToWidget(&scene->widgets[baseIndex], &scene->widgets[child1Index]);
	addChildToWidget(&scene->widgets[baseIndex], &scene->widgets[child2Index]);
	addChildToWidget(&scene->widgets[child2Index], &scene->widgets[child3Index]);
	addChildToWidget(&scene->widgets[child2Index], &scene->widgets[child4Index]);
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
