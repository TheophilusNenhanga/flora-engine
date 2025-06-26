#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "app_state.h"
#include "flora_constants.h"
#include "input_handling.h"
#include "scene.h"

void render(ApplicationState *state) {
  SDL_SetRenderDrawColor(state->mainRenderer, 0, 0, 0, 255);
  SDL_RenderClear(state->mainRenderer);
  SDL_RenderPresent(state->mainRenderer);
}

void applicationLoop(ApplicationState *state, SceneManager *sceneManager) {
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

int main(int argc, char *args[]) {
  SceneManager sceneManager = {NULL, NULL};
  EventQueue eventQueue = {NULL, 0, 0};

  if (!initEventQueue(&eventQueue)) {
    return FLORA_ENGINE_FATAL;
  }

  ApplicationState appState = {
      .mainRenderer = NULL,
      .mainWindow = NULL,
      .lastFrameTime = 0,
      .windowWidth = 800,
      .windowHeight = 600,
      .eventQueue = eventQueue,
  };

  FloraScene floraScene;
  initFloraScene(&floraScene, &appState);

  if (!initApplicationState(&appState)) {
    return FLORA_ENGINE_FATAL;
  }
  initSceneManager(&sceneManager, &appState);
  setCurrentScene(&sceneManager, &floraScene);

  appState.running = true;

  applicationLoop(&appState, &sceneManager);

  cleanupSceneManager(&sceneManager);

  if (!destroyWindow(&appState)) {
    return FLORA_ENGINE_FATAL;
  }

  if (!cleanupApplicationState(&appState)) {
    return FLORA_ENGINE_FATAL;
  }

  return FLORA_ENGINE_SUCCESS;
}
