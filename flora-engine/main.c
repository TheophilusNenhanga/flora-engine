#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "game_state.h"
#include "input_handling.h"
#include "scene.h"

void render(GameState* state) {
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
	SDL_RenderClear(state->renderer);
	SDL_RenderPresent(state->renderer);
}

void gameLoop(GameState* state, SceneManager* sceneManager) {
	while (state->running)
	{
		uint64_t frameStart = SDL_GetTicks();
		getInput(state);
		
		updateScene(sceneManager);
		
		renderScene(sceneManager);
		
		uint64_t frameEnd = SDL_GetTicks();

		state->deltaTime = (frameEnd - frameStart) / 1000.0f;
		state->lastFrameTime = frameEnd;
	}
}

int main(int argc, char* args[]) {

	SceneManager sceneManager = {NULL, NULL, NULL};

	GameState gameState = {
	.renderer = NULL,
	.window = NULL,
	.lastFrameTime = 0
	};

	
	if (!initGameState(&gameState)) return 1;
	initSceneManager(&sceneManager, &gameState);

	gameState.running = true;

	gameLoop(&gameState, &sceneManager);

	if (!destroyWindow(&gameState)) {
		return 1;
	}

	if (!cleanupGameState(&gameState)) return 1;
	return 0;
}
