#include <SDL2/SDL.h>
#include <stdio.h>
#include "init.h"
#include "graphics.h"

// Main game loop
void main_loop(SDL_Renderer *renderer) {
    int running = 1;
    SDL_Event event;

    // Load an example image
    SDL_Texture *image = load_texture("example.png", renderer);
    if (!image) {
        return;
    }

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render the image to fill the entire window
        render_texture_fullscreen(image, renderer, 1280, 720);

        // Render a dialog box
        render_dialog_box(renderer, "This is a dialog box.", 50, 720 - 150, 1280 - 100, 100);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Free the image texture
    SDL_DestroyTexture(image);
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize window and renderer
    if (initialize_window(&window, &renderer) != 0) {
        return 1;
    }

    // Enter the main game loop
    main_loop(renderer);

    // Clean up resources and quit
    cleanup(window, renderer);

    return 0;
}
