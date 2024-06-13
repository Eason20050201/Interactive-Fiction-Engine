#include <SDL2/SDL.h>
#include "graphics.h"
#include "event_handler.h"

// Screen identifiers
#define SCREEN_LOGIN 0
#define SCREEN_NEW_GAME 1
#define SCREEN_CONTINUE_GAME 2
#define SCREEN_GAME_LOOP 3

// Define window width and height as macros
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

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
                        *current_screen = SCREEN_NEW_GAME;
                        render_game_screen(renderer, game_state);

                        // render_new_game_screen(renderer, game_state);
                    } else if (y >= 400 && y <= 450) {
                        // continue game 
                        // maybe find a last record txt to get last event

                        *current_screen = SCREEN_CONTINUE_GAME;
                        render_game_screen(renderer, game_state);

                        // render_continue_game_screen(renderer, game_state);
                    }
                }
            } else if (*current_screen == SCREEN_GAME_LOOP) {
                if (x >= 10 && x <= 60 && y >= WINDOW_HEIGHT - 60 && y <= WINDOW_HEIGHT - 10) {
                    handle_inventory_icon_click(renderer, game_state);
                } else if( !game_state->inventory_visible ) {
                    render_game_screen(renderer, game_state);

                    handle_option_buttons(renderer, event, game_state);
                }
            }
        }
    }
}

void render_game_screen(SDL_Renderer *renderer, GameState *game_state) {
    
    // need a function to search_event( game_state->event); to replace below image, text, etc
    /* like
    search_event( game_state ) {
        game_state->scene = ...
        game_state->character = ...
        game_state->dialog_text = next_dialog;
        game_state->option1_text = next_option_text;
        game_state->option2_text = next_option_text;
        game_state->option3_text = next_option_text;
    }
    */

    game_state->current_image = load_texture("example.png", renderer);
    
    // for test
    if( game_state->event ) {
        // game_state->current_image = load_texture("A.png", renderer);
        SDL_Texture *next_image = load_texture("A.png", renderer);
        fade_in(renderer, next_image, 50);
        SDL_DestroyTexture(game_state->current_image);
        // crossfade(renderer, game_state->current_image, next_image, 50); // Use crossfade effect
        game_state->current_image = load_texture("A.png", renderer);  
    }

    game_state->dialog_text = "開始新遊戲。";
    game_state->option1_text = "選項1";
    game_state->option2_text = "選項2";
    game_state->option3_text = "選項3";
    game_state->next_image1 = "A.png";
    game_state->next_image2 = "B.png";
    game_state->next_image3 = "computer.png";
    
    SDL_RenderClear(renderer);  // Clear the renderer before drawing new content
    render_texture_fullscreen(game_state->current_image, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    render_dialog_box(renderer, game_state->dialog_text, 50, WINDOW_HEIGHT - 150, WINDOW_WIDTH - 100, 100);


    render_button(renderer, game_state->option1_text, 340, 100, 200, 50);
    render_button(renderer, game_state->option2_text, 540, 100, 200, 50);
    render_button(renderer, game_state->option3_text, 740, 100, 200, 50);
    render_inventory_icon(renderer, 10, WINDOW_HEIGHT - 60); // Render inventory icon

    if (game_state->inventory_visible) {
        const char *items[] = {"道具1", "道具2", "道具3"};
        int num_items = sizeof(items) / sizeof(items[0]);
        render_inventory(renderer, 100, 100, 400, 300, items, num_items); // Example position and size
    }

    SDL_RenderPresent(renderer);
}


// Function to handle option buttons
void handle_option_buttons(SDL_Renderer *renderer, SDL_Event *event, GameState *game_state) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int next_event = 0;

        if (x >= 340 && x <= 540 && y >= 100 && y <= 150) {
            next_event = 1;
            game_state->event = game_state->choice_a; 
            
            // for test
            // game_state->current_image = load_texture(game_state->next_image1, renderer);
            // game_state->event = "a"; 

            // next_image = load_texture(game_state->next_image1, renderer);
            // game_state->dialog_text = "你選擇了選項1。";
        } else if (x >= 540 && x <= 740 && y >= 100 && y <= 150) {
            next_event = 1;
            game_state->event = game_state->choice_b; 

            // next_image = load_texture(game_state->next_image2, renderer);
            // game_state->dialog_text = "你選擇了選項2。";
        } else if (x >= 740 && x <= 940 && y >= 100 && y <= 150) {
            next_event = 1;
            game_state->event = game_state->choice_c; 

            // next_image = load_texture(game_state->next_image3, renderer);
            // game_state->dialog_text = "你選擇了選項3。";
        }
        else {
            next_event = 1;
            game_state->event = "a"; 
            // game_state->event = game_state->next_event;

        }

        if (next_event) {
            render_game_screen(renderer, game_state);
    //         SDL_DestroyTexture(game_state->current_image);
    //         game_state->current_image = next_image;
    // 
    //         SDL_RenderClear(renderer);  // Clear the renderer before drawing new content
    //         render_texture_fullscreen(game_state->current_image, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    //         render_dialog_box(renderer, game_state->dialog_text, 50, WINDOW_HEIGHT - 150, WINDOW_WIDTH - 100, 100);
    //         render_button(renderer, "下一步1", 340, 100, 200, 50);
    //         render_button(renderer, "下一步2", 540, 100, 200, 50);
    //         render_button(renderer, "下一步3", 740, 100, 200, 50);
    //
    //         SDL_RenderPresent(renderer);
        }
    }
}

void handle_inventory_icon_click(SDL_Renderer *renderer, GameState *game_state) {
    game_state->inventory_visible = !game_state->inventory_visible; // Toggle visibility

    if (game_state->inventory_visible) {
        const char *items[] = {"道具1", "道具2", "道具3"};
        int num_items = sizeof(items) / sizeof(items[0]);
        render_inventory(renderer, 100, 100, 400, 300, items, num_items); // Example position and size
    }
    else {
        // Redraw game screen without inventory
        render_texture_fullscreen(game_state->current_image, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
        
        render_inventory_icon(renderer, 10, WINDOW_HEIGHT - 60); // Render inventory icon
        render_dialog_box(renderer, game_state->dialog_text, 50, WINDOW_HEIGHT - 150, WINDOW_WIDTH - 100, 100);
        render_button(renderer, game_state->option1_text, 340, 100, 200, 50);
        render_button(renderer, game_state->option2_text, 540, 100, 200, 50);
        render_button(renderer, game_state->option3_text, 740, 100, 200, 50);
    }

    SDL_RenderPresent(renderer);
}