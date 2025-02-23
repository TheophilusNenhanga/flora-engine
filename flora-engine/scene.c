#include "scene.h"
#include <stddef.h>

void initSceneManager(SceneManager* manager, GameState* state) {
	manager->state = state;
	manager->currentScene = NULL;
}

void changeScene(SceneManager* manager, FloraScene* scene) {
	if (manager->currentScene) {
		manager->currentScene->onExit(manager->state);
		manager->previousScene = manager->currentScene;
	}
	manager->currentScene = scene;
	manager->currentScene->onEnter(manager->state);
}

void updateScene(SceneManager* manager) {
	if (manager->currentScene) {
		manager->currentScene->update(manager->state);
	}
}

void renderScene(SceneManager* manager) {
	if (manager->currentScene) {
		manager->currentScene->render(manager->state);
	}
}