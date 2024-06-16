#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "music.h"

// Function to load and play music
Mix_Music* load_music(const char *file) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "example-game/assets/%s", file);
    Mix_Music *music = Mix_LoadMUS(filepath); // 使用生成的文件路径
    if (!music) {
        printf("Failed to load music %s: %s\n", filepath, Mix_GetError());
    }
    return music;
}

// Function to play music
void play_music(Mix_Music *music) {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(music, -1);
    }
}

// Function to stop music
void stop_music() {
    Mix_HaltMusic();
}

// Function to load and play sound effect
Mix_Chunk* load_sound(const char *file) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "example-game/assets/%s", file);
    Mix_Chunk *sound = Mix_LoadWAV(filepath);
    if (!sound) {
        printf("Failed to load sound %s: %s\n", filepath, Mix_GetError());
    }
    return sound;
}

// Function to play sound effect
void play_sound(Mix_Chunk *sound) {
    Mix_PlayChannel(-1, sound, 0);
}

