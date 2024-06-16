#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>

Mix_Music* load_music(const char *file);
void play_music(Mix_Music *music);
void stop_music();
Mix_Chunk* load_sound(const char *file);
void play_sound(Mix_Chunk *sound);
void cleanup_music();

#endif // MUSIC_H

