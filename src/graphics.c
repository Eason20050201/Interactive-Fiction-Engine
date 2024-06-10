#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "graphics.h"

// Define window width and height as macros
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

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

    // Render the text inside the dialog box
    render_text(renderer, text, x + 10, y + 10, w - 20, h - 20);
}

// Function to render a button
void render_button(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h) {
    // Set button background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
    SDL_Rect button = {x, y, w, h};
    SDL_RenderFillRect(renderer, &button);

    // Set button border color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_RenderDrawRect(renderer, &button);

    // Render the text inside the button
    render_text(renderer, text, x + 10, y + 10, w - 20, h - 20);
}

// Function to render text using SDL_ttf
void render_text(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h) {
    TTF_Font *font = TTF_OpenFont("example-game/assets/NotoSansCJKtc-Regular.ttf", 24); // Load a font with size 24
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }
    SDL_Color color = {255, 255, 255, 255}; // White color
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, w);
    if (!surface) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}
