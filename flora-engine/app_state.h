#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL.h>

typedef struct {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	uint64_t lastFrameTime;
	double deltaTime;
	bool running;
} ApplicationState;

bool initApplicationState(ApplicationState* appState);
bool cleanupApplicationState(ApplicationState* appState);

bool createWindow(ApplicationState* appState);
bool destroyWindow(ApplicationState* appState);

#endif // !GAME_STATE_H


