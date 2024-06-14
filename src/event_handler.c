#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "graphics.h"
#include "event_handler.h"
#include "gamming.h"

// Function to handle events
void handle_events(SDL_Event *event, int *running, int *current_screen, SDL_Renderer *renderer, GameState *game_state) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            *running = 0;
        } else if (event->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (*current_screen == SCREEN_LOGIN) {
                if (x >= 540 && x <= 740) {
                    if (y >= 200 && y <= 250) {
                        // Handle settings option
                    } else if (y >= 300 && y <= 350) {
                        // new game
                        game_state->event = "START";
                        *current_screen = SCREEN_NEW_GAME;
                        render_game_screen(renderer, game_state);
                    } else if (y >= 400 && y <= 450) {
                        // continue game
                        *current_screen = SCREEN_CONTINUE_GAME;
                        render_game_screen(renderer, game_state);
                    }
                }
            } else if (*current_screen == SCREEN_GAME_LOOP) {
                if( strcmp(game_state->event, "END") == 0 ) {
                    *current_screen = SCREEN_END;
                }
                if (x >= 10 && x <= 60 && y >= WINDOW_HEIGHT - 60 && y <= WINDOW_HEIGHT - 10) {
                    // handle_inventory_icon_click(renderer, game_state);
                } else if( !game_state->inventory_visible ) {
                    render_game_screen(renderer, game_state);
                    handle_option_buttons(renderer, event, game_state);
                } 
            }
        }
    }
}

void render_game_screen(SDL_Renderer *renderer, GameState *game_state) {
       
    search_event( game_state );
        
    // get the image of scene
    game_state->current_image = load_texture(game_state->scene, renderer);
    game_state->character_image = load_texture(game_state->character, renderer);

    SDL_RenderClear(renderer);  // Clear the renderer before drawing new content

    render_texture_fullscreen(game_state->current_image, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    render_texture_fullscreen(game_state->character_image, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    render_dialog_box(renderer, game_state->dialogue_text, 50, WINDOW_HEIGHT - 150, WINDOW_WIDTH - 100, 150);
    render_name_box(renderer, game_state->character_name, 60, WINDOW_HEIGHT - 220, 150, 60);
    
    if( game_state->have_choice ) {
        render_button(renderer, game_state->choice_a, 350, 200, 600, 50);
        render_button(renderer, game_state->choice_b, 350, 300, 600, 50);
        render_button(renderer, game_state->choice_c, 350, 400, 600, 50);
    }
    render_inventory_icon(renderer, 10, WINDOW_HEIGHT - 60); // Render inventory icon
    
    // not yet
    // if (game_state->inventory_visible) {
    //     const char *items[] = {"道具1", "道具2", "道具3"};
    //     int num_items = sizeof(items) / sizeof(items[0]);
    //     render_inventory(renderer, 100, 100, 400, 300, items, num_items); // Example position and size
    // }

    // Render character affinity in the top left corner
    // render_character_affinity(renderer, "A.png", game_state->affect_change, 10, 10, 50);


    SDL_RenderPresent(renderer);
}

void print( GameState *game_state) {
    printf("event: %s\n", game_state->event);
    printf("next_event: %s\n", game_state->next_event);
    printf("scene: %s\n", game_state->scene);
    printf("character: %s\n", game_state->character);
    printf("affect_change: %d\n", game_state->affect_change);
    printf("choice_a: %s\n", game_state->choice_a);
    printf("choice_b: %s\n", game_state->choice_b);
    printf("choice_c: %s\n", game_state->choice_c);
    printf("dialogue_text: %s\n", game_state->dialogue_text);
    printf("option1_event: %s\n", game_state->option1_event);
    printf("option2_event: %s\n", game_state->option2_event);
    printf("option3_event: %s\n", game_state->option3_event);
}

// Function to handle option buttons
void handle_option_buttons(SDL_Renderer *renderer, SDL_Event *event, GameState *game_state) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (event->type == SDL_MOUSEBUTTONDOWN ) {
        int change_event = 0;

        printf("before:\n");
        print( game_state);
        printf("---\n");
        
        if (x >= 350 && x <= 950 && y >= 200 && y <= 250 && game_state->have_choice) {
            change_event = 1;
            // strcpy(game_state->event, game_state->option1_event);
            game_state->event = game_state->option1_event;
        } else if (x >= 350 && x <= 950 && y >= 300 && y <= 350 && game_state->have_choice) {
            change_event = 1;
            // strcpy(game_state->event, game_state->option2_event);
            game_state->event = game_state->option2_event;
        } else if (x >= 350 && x <= 950 && y >= 400 && y <= 450 && game_state->have_choice) {
            change_event = 1;
            // strcpy(game_state->event, game_state->option3_event);
            game_state->event = game_state->option3_event;
        }
        else if ( !game_state->have_choice ) {
            change_event = 1;
            // strcpy(game_state->event, game_state->next_event);
            game_state->event = game_state->next_event;
        }

        if (change_event) {
            render_game_screen(renderer, game_state);
            printf("after\n");
            print( game_state );
            printf("---\n");
        }
    }
}

// not yet
void handle_inventory_icon_click(SDL_Renderer *renderer, GameState *game_state) {
    game_state->inventory_visible = !game_state->inventory_visible; // Toggle visibility

    if (game_state->inventory_visible) {
        const char *items[] = {"道具1", "道具2", "道具3"};
        int num_items = sizeof(items) / sizeof(items[0]);
        render_inventory(renderer, 100, 100, 400, 300, items, num_items); // Example position and size
    }
    // else {
    //     // Redraw game screen without inventory
    //     render_texture_fullscreen(game_state->current_image, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    //     
    //     render_inventory_icon(renderer, 10, WINDOW_HEIGHT - 60); // Render inventory icon
    //     render_dialog_box(renderer, game_state->dialogue_text, 50, WINDOW_HEIGHT - 150, WINDOW_WIDTH - 100, 100);
    //     render_button(renderer, game_state->choice_a, 340, 100, 200, 50);
    //     render_button(renderer, game_state->choice_b, 540, 100, 200, 50);
    //     render_button(renderer, game_state->choice_c, 740, 100, 200, 50);
    // }

    SDL_RenderPresent(renderer);
}