#include <SDL.h>

#include "game_state.h"

#include <stdbool.h>
#include <stdio.h>

#include "flora_constants.h"

bool createWindow(GameState* gameState) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Log: Failed to initialize SDL.");
		return false;
	}

	gameState->window = SDL_CreateWindow(
		"Flora Engine",
		FLORA_WIDOW_WIDTH,
		FLORA_WIDOW_HEIGHT,
		0
	);

	if (!gameState->window) {
		fprintf(stderr, "Log: Failed to create window.");
		return false;
	}

	gameState->renderer = SDL_CreateRenderer(gameState->window, NULL);
	if (!gameState->renderer) {
		fprintf(stderr, "Log: Failed to create renderer.");
		return false;
	}
	printf("Log: Window created successfully.\n");
	return true;
}

bool destroyWindow(GameState* gameState) {
	SDL_DestroyRenderer(gameState->renderer);
	SDL_DestroyWindow(gameState->window);
	SDL_Quit();
	printf("Log: Window destroyed successfully.\n");
	return true;
}

bool initGameState(GameState* gameState) {
	if (!createWindow(gameState)) {
		return false;
	}
	return true;
}

bool cleanupGameState(GameState* gameState) {
	gameState->renderer = NULL;
	gameState->window = NULL;
	gameState->lastFrameTime = 0;
	gameState->deltaTime = 0;
	gameState->running = false;
	printf("Log: Game state cleaned up successfully.\n");
	return true;
}
