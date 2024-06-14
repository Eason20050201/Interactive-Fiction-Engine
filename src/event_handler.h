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
    char *character_name; //角色名字
    char *choice_a;
    char *choice_b;
    char *choice_c;
    char *dialogue_text;
    char *option1_event;
    char *option2_event;
    char *option3_event;
    char *option1_required_id;
    char *option2_required_id;
    char *option3_required_id;
    int option1_required;
    int option2_required;
    int option3_required;
    char *option1_character_id;
    char *option2_character_id;
    char *option3_character_id;
    int option1_affection_change;
    int option2_affection_change;
    int option3_affection_change;

    int have_choice;
    int inventory_visible;
} GameState;


// Function declarations
void handle_events(SDL_Event *event, int *running, int *current_screen, SDL_Renderer *renderer, GameState *game_state);

void render_game_screen(SDL_Renderer *renderer, GameState *game_state);

// void render_new_game_screen(SDL_Renderer *renderer, GameState *game_state);
// void render_continue_game_screen(SDL_Renderer *renderer, GameState *game_state);

void handle_option_buttons(SDL_Renderer *renderer, SDL_Event *event, GameState *game_state);
void handle_inventory_icon_click(SDL_Renderer *renderer, GameState *game_state);

#endif // EVENT_HANDLER_H