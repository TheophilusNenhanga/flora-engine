#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <stdint.h>

typedef enum {
	FLORA_MOUSE_MOVE,
	FLORA_MOUSE_BUTTON_DOWN,
	FLORA_MOUSE_BUTTON_UP,
	FLORA_KEY_DOWN,
	FLORA_KEY_UP,
	FLORA_QUIT,
	FLORA_UNHANDLED
} FloraEventType;

typedef struct {
	FloraEventType type;
	union {
		SDL_MouseMotionEvent mouseMotion;
		SDL_MouseButtonEvent mouseButton;
		SDL_KeyboardEvent key;
		SDL_QuitEvent quit;
		// Add more event types as needed
	} as;
} FloraEvent; 

typedef struct {
	FloraEvent** events;
	// The index of the last event added to the queue
	int back;
	// The total number of events the queue can hold
	int capacity;
	// The index of the next event to be processed
	int front;
}EventQueue;

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
bool enqueueEventQueue(EventQueue* queue, FloraEvent* event);
bool dequeueEventQueue(EventQueue* queue, FloraEvent** event);
bool isEventQueueEmpty(EventQueue* queue);

bool cleanupFloraEvent(FloraEvent* event);

#endif // !APP_STATE_H
