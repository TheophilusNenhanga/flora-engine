#include <stdio.h>
#include "input_handling.h"

static FloraEvent* newFloraEvent( SDL_Event* sdlEvent) {
	FloraEvent* floraEvent = malloc(sizeof(FloraEvent));
    
    if (!floraEvent) {
        fprintf(stderr, "Error: Failed to allocate memory for FloraEvent.\n");
        return NULL;
    }

    if (!sdlEvent) {
        fprintf(stderr, "Error: SDL_Event pointer is NULL.\n");
        return NULL;
    }
    switch (sdlEvent->type) {
    case SDL_EVENT_KEY_DOWN: {
        floraEvent->type = FLORA_KEY_DOWN;
        floraEvent->as.key = sdlEvent->key;
        break;
    }
    case SDL_EVENT_KEY_UP: {
        floraEvent->type = FLORA_KEY_UP;
        floraEvent->as.key = sdlEvent->key;
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        floraEvent->type = FLORA_MOUSE_BUTTON_DOWN;
        floraEvent->as.mouseButton = sdlEvent->button;
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        floraEvent->type = FLORA_MOUSE_BUTTON_UP;
        floraEvent->as.mouseButton = sdlEvent->button;
        break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
        floraEvent->type = FLORA_MOUSE_MOVE;
        floraEvent->as.mouseMotion = sdlEvent->motion;
        break;
    }
    case SDL_EVENT_QUIT: {
        floraEvent->type = FLORA_QUIT;
        floraEvent->as.quit = sdlEvent->quit;
        break;
    }
    default: {
        fprintf(stderr, "Error: Unsupported SDL event type: %d\n", sdlEvent->type);
        printf("Log: Unsupported SDL event type: %d\n", sdlEvent->type);
		floraEvent->type = FLORA_UNHANDLED;
        floraEvent->as.quit = (SDL_QuitEvent){0}; 
		break;
    }
    }
	return floraEvent;
}

void getInput(ApplicationState *state) {
  SDL_Event sdlEvent;

  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_EVENT_QUIT: {
		FloraEvent* floraEvent = newFloraEvent(&sdlEvent);
      enqueueEventQueue(&state->eventQueue, floraEvent);
      break;
    }
    case SDL_EVENT_KEY_DOWN: {
      switch (sdlEvent.key.key) {
      case SDLK_ESCAPE: 
      case SDLK_LEFT:
      case SDLK_RIGHT:
      case SDLK_UP:
      case SDLK_DOWN:
      case SDLK_SPACE:
      case SDLK_RETURN:
      case SDLK_BACKSPACE: {
          FloraEvent* floraEvent = newFloraEvent(&sdlEvent);
        enqueueEventQueue(&state->eventQueue, floraEvent);
        break;
      }
      }
      break;
    }
    case SDL_EVENT_KEY_UP: {
      switch (sdlEvent.key.key) {
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
	  break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        FloraEvent* floraEvent = newFloraEvent(&sdlEvent);
      enqueueEventQueue(&state->eventQueue, floraEvent);
      break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
		FloraEvent* floraEvent = newFloraEvent(&sdlEvent);
      enqueueEventQueue(&state->eventQueue, floraEvent);
      break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
        FloraEvent* floraEvent = newFloraEvent(&sdlEvent);
      enqueueEventQueue(&state->eventQueue, floraEvent);
      break;
    }
    }
  }
}
