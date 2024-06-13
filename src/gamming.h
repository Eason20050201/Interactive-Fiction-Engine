#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef struct {
    // SDL_Texture *current_scene;
    // SDL_Texture *character;
    char *event;
    char *next_event;
    char *scene;    // image name
    char *charcter; // image name
    int affect_change;
    char *choice_a;
    char *choice_b;
    char *choice_c;
    char *dialogue_text;
    char *option1_event;
    char *option2_event;
    char *option3_event;


    int inventory_visible;
} GameState;

void search_event( GameState *game_state );