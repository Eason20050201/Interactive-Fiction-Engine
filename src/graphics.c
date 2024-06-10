#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "graphics.h"

// Function to load a texture from a file
SDL_Texture* load_texture(const char *file, SDL_Renderer *renderer) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "example-game/assets/%s", file);
    SDL_Texture *texture = IMG_LoadTexture(renderer, filepath);
    if (!texture) {
        printf("Failed to load texture %s: %s\n", filepath, IMG_GetError());
    }
    return texture;
}

// Function to render a texture to fill the window
void render_texture_fullscreen(SDL_Texture *texture, SDL_Renderer *renderer, int window_width, int window_height) {
    int texture_width, texture_height;
    SDL_QueryTexture(texture, NULL, NULL, &texture_width, &texture_height);

    float window_aspect = (float)window_width / (float)window_height;
    float texture_aspect = (float)texture_width / (float)texture_height;

    SDL_Rect dest;
    if (window_aspect > texture_aspect) {
        dest.w = window_width;
        dest.h = window_width / texture_aspect;
        dest.x = 0;
        dest.y = (window_height - dest.h) / 2;
    } else {
        dest.w = window_height * texture_aspect;
        dest.h = window_height;
        dest.x = (window_width - dest.w) / 2;
        dest.y = 0;
    }

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

// Function to render a dialog box
void render_dialog_box(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h) {
    // Set dialog box background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255); // Dark blue
    SDL_Rect dialog_box = {x, y, w, h};
    SDL_RenderFillRect(renderer, &dialog_box);

    // Set text color and render the text
    // Note: You would typically use SDL_ttf to render text, but it's not covered here
    // For simplicity, this example does not include text rendering
    // SDL_RenderDrawText(renderer, text, x + 10, y + 10); // Example placeholder
}
