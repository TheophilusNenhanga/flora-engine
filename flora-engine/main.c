#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

#include "flora_constants.h"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;
} GameState;

bool createWindow(GameState* gameState) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Failed to initialize SDL.");
		return false;
	}

	gameState->window = SDL_CreateWindow(
		"Flora Engine", 
		FLORA_WIDOW_WIDTH, 
		FLORA_WIDOW_HEIGHT, 
		0
	);

	if (!gameState->window) {
		fprintf(stderr, "Failed to create window.");
		return false;
	}

	gameState->renderer = SDL_CreateRenderer(gameState->window, NULL);
	if (!gameState->renderer) {
		fprintf(stderr, "Failed to create renderer.");
		return false;
	}
	printf("Window created successfully.\n");
	return true;
}

bool destroyWindow(GameState* gameState) {
	SDL_DestroyRenderer(gameState->renderer);
	SDL_DestroyWindow(gameState->window);
	SDL_Quit();
	printf("Window destroyed successfully.\n");
	return true;
}


void getInput(GameState* state) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_EVENT_QUIT: {
			state->running = false;
			break;
		}
		case SDL_EVENT_KEY_DOWN: {
			if (event.key.key == SDLK_ESCAPE) {
				state->running = false;
			}
			break;
		}
	}
}

void update(GameState* state) {

}

void render(GameState* state) {

}

void gameLoop(GameState* state) {
	while (state->running)
	{
		getInput(state);
		update(state);
		render(state);
	}
}

void setup(GameState* state) {
}


int main(int argc, char* args[]) {

	GameState gameState = { false };

	if (!createWindow(&gameState)) {
		return 1;
	}

	gameState.running = true;

	setup(&gameState);

	gameLoop(&gameState);


	if (!destroyWindow(&gameState)) {
		return 1;
	}

}
