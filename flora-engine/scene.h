#ifndef SCENE_H
#define SCENE_H

#include "app_state.h"

typedef struct FloraWidget FloraWidget;
typedef struct FloraScene FloraScene;

struct FloraWidget{
	double posX;
	double posY;
	double width;
	double height;
	bool isVisible;

	void (*update)(FloraWidget* widget, ApplicationState* state);
	void (*render)(FloraWidget* widget, ApplicationState* state);
	void (*onClick)(FloraWidget* widget, ApplicationState* state, int mouseX, int mouseY);
};

struct FloraScene {
	void (*onCreate)(ApplicationState* state, FloraScene* scene);
	void (*onDestroy)(ApplicationState* state, FloraScene* scene);
	FloraWidget* widgets;
	int widgetCount;
};

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

#endif // !SCENE_H
