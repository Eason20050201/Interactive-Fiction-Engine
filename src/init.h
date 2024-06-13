#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL.h>

int initialize_window(SDL_Window **window, SDL_Renderer **renderer);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif // INIT_H