#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "init.h"
#include "graphics.h"
#include "event_handler.h"

// Screen identifiers
#define SCREEN_LOGIN 0
#define SCREEN_NEW_GAME 1

// Function to render the login screen
void render_login_screen(SDL_Renderer *renderer) {
    // Load the background image
    SDL_Texture *background = load_texture("enter_background.png", renderer);
    if (!background) {
        // Exit the program if the background image cannot be loaded
        printf("Error: Could not load enter_background.png\n");
        exit(1);
    }

    // Render the background image
    render_texture_fullscreen(background, renderer, 1280, 720);

    // Render buttons
    render_button(renderer, "新遊戲", 540, 200, 200, 50);
    render_button(renderer, "設定", 540, 300, 200, 50);
    render_button(renderer, "進入遊戲", 540, 400, 200, 50);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Free the background texture
    SDL_DestroyTexture(background);
}

// Main game loop
void main_loop(SDL_Renderer *renderer) {
    int running = 1;
    int current_screen = SCREEN_LOGIN;
    SDL_Event event;

    // Render the login screen initially
    render_login_screen(renderer);

    while (running) {
        handle_events(&event, &running, &current_screen, renderer);

        if (current_screen == SCREEN_LOGIN) {
            render_login_screen(renderer);
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL and create window and renderer
    if (initialize_window(&window, &renderer) != 0) {
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf Initialization failed: %s\n", TTF_GetError());
        cleanup(window, renderer);
        return 1;
    }

    // Enter the main game loop
    main_loop(renderer);

    // Clean up resources and quit
    TTF_Quit();
    cleanup(window, renderer);

    return 0;
}
