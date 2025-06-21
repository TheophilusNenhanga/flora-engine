#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "app_state.h"
#include "input_handling.h"
#include "scene.h"

void render(ApplicationState* state) {
	SDL_SetRenderDrawColor(state->mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(state->mainRenderer);
	SDL_RenderPresent(state->mainRenderer);
}

void applicationLoop(ApplicationState* state, SceneManager* sceneManager) {
	while (state->running) {
		uint64_t frameStart = SDL_GetTicks();
		
		getInput(state);
		updateScene(sceneManager);
		SDL_SetRenderDrawColor(state->mainRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(state->mainRenderer);
		renderScene(sceneManager);
		SDL_RenderPresent(state->mainRenderer);
		
		uint64_t frameEnd = SDL_GetTicks();

		state->deltaTime = (frameEnd - frameStart) / 1000.0f;
		state->lastFrameTime = frameEnd;
	}
}

int main(int argc, char* args[]) {
	SceneManager sceneManager = {NULL, NULL};
	ApplicationState appState = {
	.mainRenderer = NULL,
	.mainWindow = NULL,
	.lastFrameTime = 0,
	.windowWidth = 800,
	.windowHeight = 600,
	};

	FloraScene floraScene;
	initFloraScene(&floraScene, &appState);
	
	if (!initApplicationState(&appState)) return 1;
	initSceneManager(&sceneManager, &appState);
	setCurrentScene(&sceneManager, &floraScene);

	appState.running = true;

	applicationLoop(&appState, &sceneManager);

	if (!destroyWindow(&appState)) {
		return 1;
	}

	if (!cleanupApplicationState(&appState)) return 1;
	return 0;
}
