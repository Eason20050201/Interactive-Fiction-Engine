#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

SDL_Texture* load_texture(const char *file, SDL_Renderer *renderer);
void render_texture_fullscreen(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height);
void render_dialog_box(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h);

#endif // GRAPHICS_H
