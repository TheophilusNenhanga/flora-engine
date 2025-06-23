#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

void initSceneManager(SceneManager *manager, ApplicationState *state) {
  manager->state = state;
  manager->currentScene = NULL;
}

void setCurrentScene(SceneManager *manager, FloraScene *scene) {
  manager->currentScene = scene;
  scene->onCreate(manager->state, scene);
}

void updateScene(SceneManager *manager) {
  if (manager->currentScene) {
    FloraScene *scene = manager->currentScene;
    for (int i = 0; i < scene->widgetCount; i++) {
      FloraWidget *widget = &scene->widgets[i];
      if (widget->isVisible) {
        widget->update(widget, manager->state);
      }
    }

    while (!isEventQueueEmpty(&manager->state->eventQueue)) {
      SDL_Event event;
      if (dequeueEventQueue(&manager->state->eventQueue, &event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT: {
          manager->state->running = false;
          break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
          for (int i = 0; i < scene->widgetCount; i++) {
            FloraWidget *widget = &scene->widgets[i];
            if (widget->isVisible && widget->onClick &&
                widgetContainsPoint(widget, event.button.x, event.button.y)) {
              widget->onClick(widget, manager->state, &event.button);
              break; // Stop after the first widget that handles the click
            }
          }
        default: {
          break;
        }
        }
        }
      }
    }
  }
}

void renderScene(SceneManager *manager) {
  if (manager->currentScene) {
    FloraScene *scene = manager->currentScene;
    for (int i = 0; i < scene->widgetCount; i++) {
      FloraWidget *widget = &scene->widgets[i];
      if (widget->isVisible) {
        widget->render(widget, manager->state);
      }
    }
  }
}

void addWidgetToScene(FloraScene *scene, FloraWidget widget) {
  if (!scene) {
    return;
  }

  if (scene->widgetCount + 1 >= scene->widgetCapacity) {
    int newCapacity = scene->widgetCapacity * 2;
    FloraWidget *newWidgets = (FloraWidget *)realloc(
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

void baseCreateScene(ApplicationState *state, FloraScene *scene) {
  addWidgetToScene(scene,
                   (FloraWidget){.posX = 50,
                                 .posY = 50,
                                 .width = 100,
                                 .height = 100,
                                 .isVisible = true,
                                 .update = baseWidgetUpdate,
                                 .render = baseWidgetRender,
                                 .onClick = baseWidgetOnClick,
                                 .colour = (FloraColour){
                                     .r = 25, .g = 132, .b = 191, .a = 255}});
  addWidgetToScene(scene,
                   (FloraWidget){.posX = 250,
                                 .posY = 70,
                                 .width = 10,
                                 .height = 50,
                                 .isVisible = true,
                                 .update = baseWidgetUpdate,
                                 .render = baseWidgetRender,
                                 .onClick = baseWidgetOnClick,
                                 .colour = (FloraColour){
                                     .r = 213, .g = 171, .b = 12, .a = 255}});
  addWidgetToScene(scene,
                   (FloraWidget){.posX = 750,
                                 .posY = 500,
                                 .width = 30,
                                 .height = 60,
                                 .isVisible = true,
                                 .update = baseWidgetUpdate,
                                 .render = baseWidgetRender,
                                 .onClick = baseWidgetOnClick,
                                 .colour = (FloraColour){
                                     .r = 255, .g = 122, .b = 222, .a = 255}});
  addWidgetToScene(
      scene, (FloraWidget){
                 .posX = 400,
                 .posY = 55,
                 .width = 120,
                 .height = 20,
                 .isVisible = true,
                 .update = baseWidgetUpdate,
                 .render = baseWidgetRender,
                 .onClick = baseWidgetOnClick,
                 .colour = (FloraColour){.r = 55, .g = 22, .b = 66, .a = 255}});
}

void baseDestroyScene(ApplicationState *state, FloraScene *scene) {
  if (scene->widgets) {
    free(scene->widgets);
    scene->widgets = NULL;
    scene->widgetCount = 0;
    printf("Log: Scene destroyed and memory freed.\n");
  }
  scene->onCreate = NULL;
  scene->onDestroy = NULL;
}

void initFloraScene(FloraScene *scene, ApplicationState *state) {
  scene->onCreate = baseCreateScene;
  scene->onDestroy = baseDestroyScene;
  scene->widgetCount = 0;
  scene->widgetCapacity = 8;
  scene->widgets =
      (FloraWidget *)malloc(scene->widgetCapacity * sizeof(FloraWidget));
  if (!scene->widgets) {
    printf("Log: Failed to allocate memory for scene widgets. Aborting!\n");
    fprintf(stderr, "Error: Failed to allocate memory for scene widgets.\n");
    exit(1);
  }
}
