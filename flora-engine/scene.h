#ifndef SCENE_H
#define SCENE_H

#include "game_state.h"

typedef struct {
	void (*onEnter)(GameState* state);
	void (*onExit)(GameState* state);
	void (*update)(GameState* state);
	void (*render)(GameState* state);
} FloraScene;

typedef struct {
	FloraScene* previousScene;
	FloraScene* currentScene;
	GameState* state;
} SceneManager;

void initSceneManager(SceneManager* manager, GameState* state);
void changeScene(SceneManager* manager, FloraScene* scene);
void updateScene(SceneManager* manager);
void renderScene(SceneManager* manager);

#endif // !SCENE_H
