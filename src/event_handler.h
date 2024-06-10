#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>

// Function declarations
void handle_events(SDL_Event *event, int *running, int *current_screen, SDL_Renderer *renderer);
void render_new_game_screen(SDL_Renderer *renderer);

#endif // EVENT_HANDLER_H
