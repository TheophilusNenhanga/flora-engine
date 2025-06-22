#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdbool.h>
#include <SDL.h>
#include <stdint.h>

typedef struct {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	uint64_t lastFrameTime;
	double deltaTime;
	bool running;
	int windowWidth;
	int windowHeight;
}ApplicationState;

bool initApplicationState(ApplicationState* appState);
bool cleanupApplicationState(ApplicationState* appState);

bool createWindow(ApplicationState* appState);
bool destroyWindow(ApplicationState* appState);

#endif // !APP_STATE_H
