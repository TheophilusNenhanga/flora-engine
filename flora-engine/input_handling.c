#include "input_handling.h"

void getInput(GameState* state) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT: {
			state->running = false;
			break;
		}
		case SDL_EVENT_KEY_DOWN: {
			switch (event.key.key) {
			case SDLK_ESCAPE: {
				state->running = false;
				break;
			}
			case SDLK_LEFT: {
				break;
			}
			case SDLK_RIGHT: {
				break;
			}
			case SDLK_UP: {
				break;
			}
			case SDLK_DOWN: {
				break;
			}
			break;
			}
		}
		}
	}
}