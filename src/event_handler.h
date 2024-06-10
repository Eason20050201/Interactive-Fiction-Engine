#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture *current_image;
    char *dialog_text;
    char *option1_text;
    char *option2_text;
    char *option3_text;
    char *next_image1;
    char *next_image2;
    char *next_image3;
} GameState;

// Function declarations
void handle_events(SDL_Event *event, int *running, int *current_screen, SDL_Renderer *renderer, GameState *game_state);
void render_new_game_screen(SDL_Renderer *renderer, GameState *game_state);
void render_continue_game_screen(SDL_Renderer *renderer, GameState *game_state);
void handle_option_buttons(SDL_Renderer *renderer, SDL_Event *event, GameState *game_state);

#endif // EVENT_HANDLER_H
