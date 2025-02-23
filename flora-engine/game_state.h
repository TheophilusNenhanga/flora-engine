#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL.h>

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint64_t lastFrameTime;
	double deltaTime;
	bool running;
} GameState;

bool initGameState(GameState* gameState);
bool cleanupGameState(GameState* gameState);

bool createWindow(GameState* gameState);
bool destroyWindow(GameState* gameState);

#endif // !GAME_STATE_H


