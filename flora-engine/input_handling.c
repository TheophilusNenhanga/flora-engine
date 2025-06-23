#include "input_handling.h"

void getInput(ApplicationState *state) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT: {
      enqueueEventQueue(&state->eventQueue, event);
      break;
    }
    case SDL_EVENT_KEY_DOWN: {
      switch (event.key.key) {
      case SDLK_ESCAPE: {
        state->running = false;
        break;
      }
      case SDLK_LEFT:
      case SDLK_RIGHT:
      case SDLK_UP:
      case SDLK_DOWN:
      case SDLK_SPACE:
      case SDLK_RETURN:
      case SDLK_BACKSPACE: {
        enqueueEventQueue(&state->eventQueue, event);
        break;
      }
      }
      break;
    }
    case SDL_EVENT_KEY_UP: {
      switch (event.key.key) {
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
      }
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      enqueueEventQueue(&state->eventQueue, event);
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      enqueueEventQueue(&state->eventQueue, event);
      break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
      enqueueEventQueue(&state->eventQueue, event);
      break;
    }
    }
  }
}
