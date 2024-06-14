#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef struct {
    char *event;
    char *scene;    // image name
    char *character; // image name
    char *dialogue_text;
    char *next_event;
    char *choice_a;
    char *choice_b;
    char *choice_c;
    char *option1_event;
    char *option2_event;
    char *option3_event;


    int have_choice;
    int inventory_visible;
} GameState;

void search_event( GameState *game_state );