// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void set_player_name(SDL_Renderer *renderer, char *player_name);
void replaceSubstring(char *source, const char *target, const char *replacement);

#endif // PLAYER_H
