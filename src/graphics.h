#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

SDL_Texture* load_texture(const char *file, SDL_Renderer *renderer);
void render_texture_fullscreen(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height);
void render_dialog_box(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h);
void render_name_box(SDL_Renderer *renderer, const char *name, int x, int y, int w, int h);
void render_button(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h);
void render_text(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h, int R, int G, int B);
void render_inventory_icon(SDL_Renderer *renderer, int x, int y);
void render_inventory(SDL_Renderer *renderer, int x, int y, int w, int h, int num_items);
void render_character_affinity(SDL_Renderer *renderer, const char *image_path, int affinity, int x, int y, int diameter);
void fade_in(SDL_Renderer *renderer, SDL_Texture *texture, int duration);
void fade_out(SDL_Renderer *renderer, SDL_Texture *texture, int duration);
void crossfade(SDL_Renderer *renderer, SDL_Texture *old_texture, SDL_Texture *new_texture, int duration);
void render_text_slowly(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h, int delay);
void render_home_icon(SDL_Renderer *renderer, int x, int y);

#endif // GRAPHICS_H
