#ifndef SCENE_H
#define SCENE_H

#include "app_state.h"
#include "flora_widget.h"

typedef struct FloraScene {
	void (*onCreate)(ApplicationState* state, struct FloraScene* scene);
	void (*onDestroy)(ApplicationState* state, struct FloraScene* scene);
	FloraWidget* widgets;
	int widgetCount;
	int widgetCapacity;
} FloraScene;

typedef struct {
	FloraScene* currentScene;
	ApplicationState* state;
} SceneManager;

void initSceneManager(SceneManager* manager, ApplicationState* state);
void renderScene(SceneManager* manager);
void updateScene(SceneManager* manager);
void baseCreateScene(ApplicationState* state, FloraScene* scene);
void initFloraScene(FloraScene* scene, ApplicationState* state);
void setCurrentScene(SceneManager* manager, FloraScene* scene);
void cleanupSceneManager(SceneManager* manager);

#endif // !SCENE_H
