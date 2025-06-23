#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdbool.h>
#include <SDL.h>
#include <stdint.h>

typedef struct {
	SDL_Event* events;
	// The index of the last event added to the queue
	int back;
	// The total number of events the queue can hold
	int capacity;
	// The index of the next event to be processed
	int front;
}EventQueue;

typedef struct {
	SDL_Event event;
	enum {
		MOUSE_MOVE,
		MOUSE_BUTTON_DOWN,
		MOUSE_BUTTON_UP,
		KEY_DOWN,
		KEY_UP,
	} type;
	int mouseX;
	int mouseY;
	int keyCode;
} FloraEvent; // Maybe we can use this event instead of SDL_Event directly

typedef struct {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	uint64_t lastFrameTime;
	double deltaTime;
	int windowWidth;
	int windowHeight;
	EventQueue eventQueue;
	bool running;
}ApplicationState;

bool initApplicationState(ApplicationState* appState);
bool cleanupApplicationState(ApplicationState* appState);

bool createWindow(ApplicationState* appState);
bool destroyWindow(ApplicationState* appState);

bool initEventQueue(EventQueue* queue);
bool cleanupEventQueue(EventQueue* queue);
bool enqueueEventQueue(EventQueue* queue, SDL_Event event);
bool dequeueEventQueue(EventQueue* queue, SDL_Event* event);
bool isEventQueueEmpty(EventQueue* queue);

#endif // !APP_STATE_H
