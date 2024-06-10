#include <SDL2/SDL.h>
#include "graphics.h"
#include "event_handler.h"

// Screen identifiers
#define SCREEN_LOGIN 0
#define SCREEN_NEW_GAME 1

// Function to handle events
void handle_events(SDL_Event *event, int *running, int *current_screen, SDL_Renderer *renderer) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            *running = 0;
        } else if (event->type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (*current_screen == SCREEN_LOGIN) {
                if (x >= 540 && x <= 740) {
                    if (y >= 200 && y <= 250) {
                        *current_screen = SCREEN_NEW_GAME;
                        render_new_game_screen(renderer);
                    }
                }
            }
        }
    }
}

// Function to render the new game screen
void render_new_game_screen(SDL_Renderer *renderer) {
    SDL_Texture *image = load_texture("example.png", renderer);
    if (!image) {
        printf("Error: Could not load example.png\n");
        return;
    }

    render_texture_fullscreen(image, renderer, 1280, 720);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(image);
}
