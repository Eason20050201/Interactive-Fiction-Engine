#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "macro.h"
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Dark black
    SDL_Rect dialog_box = {x, y, w, h};
    SDL_RenderFillRect(renderer, &dialog_box);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180); // Dark black
    SDL_RenderDrawRect(renderer, &dialog_box);

    // Render the text inside the dialog box
    render_text(renderer, text, x + 20, y + 10, w - 20, h - 20);
}

void render_name_box(SDL_Renderer *renderer, const char *name, int x, int y, int w, int h){
    // Set dialog box background color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Dark black
    SDL_Rect dialog_box = {x, y, w, h};
    SDL_RenderFillRect(renderer, &dialog_box);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180); // Dark black
    SDL_RenderDrawRect(renderer, &dialog_box);

    // Render the text inside the dialog box
    render_text(renderer, name, x + 10, y + 10, w - 20, h - 20);
}

// Function to render a button
void render_button(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h) {
    // Set button background color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Blue
    SDL_Rect button = {x, y, w, h};
    SDL_RenderFillRect(renderer, &button);

    // Set button border color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180); // White
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

void render_inventory_icon(SDL_Renderer *renderer, int x, int y) {
    SDL_Texture *icon = load_texture("inventory_icon.png", renderer);
    if (!icon) {
        printf("Error: Could not load inventory_icon.png\n");
        return;
    }

    SDL_Rect dest = {x, y, 50, 50}; // Example size, adjust as needed
    SDL_RenderCopy(renderer, icon, NULL, &dest);
    SDL_DestroyTexture(icon);
}

void render_inventory(SDL_Renderer *renderer, int x, int y, int w, int h, const char **items, int num_items) {
    // Render inventory background
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark grey background
    SDL_Rect inventory_box = {x, y, w, h};
    SDL_RenderFillRect(renderer, &inventory_box);

    // Render each item
    for (int i = 0; i < num_items; ++i) {
        render_text(renderer, items[i], x + 10, y + 10 + i * 30, w - 20, 30);
    }
}

void fade_in(SDL_Renderer *renderer, SDL_Texture *texture, int duration) {
    Uint8 alpha = 0;
    int step = 255 / duration;

    for (int i = 0; i <= duration; i++) {
        SDL_SetTextureAlphaMod(texture, alpha);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        alpha += step;
        SDL_Delay(10); // Adjust delay for smoothness
    }
}

void fade_out(SDL_Renderer *renderer, SDL_Texture *texture, int duration) {
    Uint8 alpha = 255;
    int step = 255 / duration;

    for (int i = 0; i <= duration; i++) {
        SDL_SetTextureAlphaMod(texture, alpha);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        alpha -= step;
        SDL_Delay(10); // Adjust delay for smoothness
    }
}

void crossfade(SDL_Renderer *renderer, SDL_Texture *old_texture, SDL_Texture *new_texture, int duration) {
    Uint8 alpha_old = 255;
    Uint8 alpha_new = 0;
    int step = 255 / duration;

    for (int i = 0; i <= duration; i++) {
        SDL_SetTextureAlphaMod(old_texture, alpha_old);
        SDL_SetTextureAlphaMod(new_texture, alpha_new);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, old_texture, NULL, NULL);
        SDL_RenderCopy(renderer, new_texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        alpha_old -= step;
        alpha_new += step;
        SDL_Delay(10); // Adjust delay for smoothness
    }
}

void render_text_slowly(SDL_Renderer *renderer, const char *text, int x, int y, int w, int h, int delay) {
    TTF_Font *font = TTF_OpenFont("example-game/assets/NotoSansCJKtc-Regular.ttf", 24); // Load a font with size 24
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color color = {255, 255, 255, 255}; // White color
    char temp[1024] = {0}; // Temporary buffer to hold the substring
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    for (int i = 0; text[i] != '\0'; ++i) {
        strncat(temp, &text[i], 1);

        surface = TTF_RenderUTF8_Blended_Wrapped(font, temp, color, w);
        if (!surface) {
            printf("Failed to create text surface: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            return;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("Failed to create text texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            TTF_CloseFont(font);
            return;
        }

        SDL_Rect dest = {x, y, surface->w, surface->h};
        
        SDL_RenderCopy(renderer, texture, NULL, &dest); // Only update text
        SDL_RenderPresent(renderer);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        SDL_Delay(delay);  // Delay to create the typing effect
    }

    TTF_CloseFont(font);
}


void render_character_affinity(SDL_Renderer *renderer, const char *image_path, int affinity, int x, int y, int diameter) {
    // Load character image
    SDL_Texture *texture = load_texture(image_path, renderer);
    if (!texture) {
        printf("Error: Could not load %s\n", image_path);
        return;
    }

    // Render the circular headshot
    SDL_Rect dest = {x, y, diameter, diameter};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    // Render the affinity value
    TTF_Font *font = TTF_OpenFont("example-game/assets/NotoSansCJKtc-Regular.ttf", 24); // Load a font with size 24
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyTexture(texture);
        return;
    }

    char affinity_text[10];
    snprintf(affinity_text, sizeof(affinity_text), "%d", affinity);

    SDL_Color color = {255, 255, 255, 255}; // White color
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, affinity_text, color);
    if (!surface) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyTexture(texture);
        return;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!text_texture) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        SDL_DestroyTexture(texture);
        return;
    }
    
    SDL_Rect text_rect = {x + diameter + 10, y + (diameter - surface->h) / 2, surface->w, surface->h}; // Position text next to the headshot
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
}