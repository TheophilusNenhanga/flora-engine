#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "app_state.h"

bool createWindow(ApplicationState *appState) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Log: Failed to initialize SDL.");
    return false;
  }

  appState->mainWindow = SDL_CreateWindow("Flora Engine", appState->windowWidth,
                                          appState->windowHeight, 0);

  if (!appState->mainWindow) {
    fprintf(stderr, "Log: Failed to create mainWindow.");
    return false;
  }

  appState->mainRenderer = SDL_CreateRenderer(appState->mainWindow, NULL);
  if (!appState->mainRenderer) {
    fprintf(stderr, "Log: Failed to create mainRenderer.");
    return false;
  }
  printf("Log: Window created successfully.\n");
  return true;
}

bool destroyWindow(ApplicationState *appState) {
  SDL_DestroyRenderer(appState->mainRenderer);
  SDL_DestroyWindow(appState->mainWindow);
  SDL_Quit();
  printf("Log: Window destroyed successfully.\n");
  return true;
}

bool initApplicationState(ApplicationState *appState) {
  if (!createWindow(appState)) {
    return false;
  }
  return true;
}

bool cleanupApplicationState(ApplicationState *appState) {
  appState->mainRenderer = NULL;
  appState->mainWindow = NULL;
  appState->lastFrameTime = 0;
  appState->deltaTime = 0;
  appState->running = false;
  if (!cleanupEventQueue(&appState->eventQueue)) {
    fprintf(stderr, "Error: Failed to clean up event queue.\n");
    return false;
  }
  printf("Log: Game state cleaned up successfully.\n");
  return true;
}

bool initEventQueue(EventQueue *queue) {
  if (!queue)
    return false;
  queue->capacity = 8;
  queue->front = 0;
  queue->back = 0;
  queue->events = malloc(queue->capacity * sizeof(SDL_Event));
  if (!queue->events) {
    fprintf(stderr, "Error: Failed to allocate memory for event queue.\n");
    printf("Log: Failed to allocate memory for event queue. Shutting Down.\n");
    return false;
  }
  printf("Log: Event queue initialized with capacity: %d\n", queue->capacity);
  return true;
}

bool cleanupEventQueue(EventQueue *queue) {
  if (!queue || !queue->events)
    return false;
  free(queue->events);
  queue->events = NULL;
  queue->front = 0;
  queue->capacity = 0;
  queue->back = 0;
  printf("Log: Event queue cleaned up successfully.\n");
  return true;
}

bool enqueueEventQueue(EventQueue *queue, SDL_Event event) {
  if (!queue)
    return false;
  int nextBack = (queue->back + 1) % queue->capacity;

  // Check if the queue is full
  if (nextBack == queue->front) {

    int newCapacity = queue->capacity * 2;
    SDL_Event *newEvents = malloc(newCapacity * sizeof(SDL_Event));
    if (!newEvents) {
      fprintf(stderr, "Error: Failed to allocate memory for new events.\n");
      return false;
    }

    // Copy existing events to new array
    int count = 0;
    int i = queue->front;
    while (i != queue->back) {
      newEvents[count++] = queue->events[i];
      i = (i + 1) % queue->capacity;
    }

    free(queue->events);
    queue->events = newEvents;
    queue->front = 0;
    queue->back = count;
    queue->capacity = newCapacity;
    printf("Log: Resized event queue to new capacity: %d\n", newCapacity);
  }

  queue->events[queue->back] = event;
  queue->back = (queue->back + 1) % queue->capacity;
  return true;
}

bool dequeueEventQueue(EventQueue *queue, SDL_Event *event) {
  if (!queue || !event)
    return false;

  // Check if queue is empty
  if (queue->front == queue->back)
    return false;

  *event = queue->events[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  return true;
}

bool isEventQueueEmpty(EventQueue *queue) {
  if (!queue)
    return true;
  return queue->front == queue->back;
}
