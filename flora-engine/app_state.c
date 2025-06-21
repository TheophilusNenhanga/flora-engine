#include <SDL.h>

#include "app_state.h"

#include <stdbool.h>
#include <stdio.h>

#include "flora_constants.h"

bool createWindow(ApplicationState* appState) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Log: Failed to initialize SDL.");
		return false;
	}

	appState->mainWindow = SDL_CreateWindow(
		"Flora Engine",
		FLORA_WIDOW_WIDTH,
		FLORA_WIDOW_HEIGHT,
		0
	);

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

bool destroyWindow(ApplicationState* appState) {
	SDL_DestroyRenderer(appState->mainRenderer);
	SDL_DestroyWindow(appState->mainWindow);
	SDL_Quit();
	printf("Log: Window destroyed successfully.\n");
	return true;
}

bool initApplicationState(ApplicationState* appState) {
	if (!createWindow(appState)) {
		return false;
	}
	return true;
}

bool cleanupApplicationState(ApplicationState* appState) {
	appState->mainRenderer = NULL;
	appState->mainWindow = NULL;
	appState->lastFrameTime = 0;
	appState->deltaTime = 0;
	appState->running = false;
	printf("Log: Game state cleaned up successfully.\n");
	return true;
}
