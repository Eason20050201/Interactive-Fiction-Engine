#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture *current_image;
    SDL_Texture *character_image;
    char *event;
    char *next_event;
    char *scene;    // image name
    char *character; // image name
    char *choice_a;
    char *choice_b;
    char *choice_c;
    char *dialogue_text;
    char *option1_event;
    char *option2_event;
    char *option3_event;

    int have_choice;
    int inventory_visible;
    int  affect_change;
} GameState;


// Function declarations
void handle_events(SDL_Event *event, int *running, int *current_screen, SDL_Renderer *renderer, GameState *game_state);

void render_game_screen(SDL_Renderer *renderer, GameState *game_state);

// void render_new_game_screen(SDL_Renderer *renderer, GameState *game_state);
// void render_continue_game_screen(SDL_Renderer *renderer, GameState *game_state);

void handle_option_buttons(SDL_Renderer *renderer, SDL_Event *event, GameState *game_state);
void handle_inventory_icon_click(SDL_Renderer *renderer, GameState *game_state);

#endif // EVENT_HANDLER_H
