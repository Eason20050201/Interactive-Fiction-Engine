#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

extern char *save_event_id;

void search_event( GameState *game_state );
void save_game(const char *filename);
void load_game(const char *filename);