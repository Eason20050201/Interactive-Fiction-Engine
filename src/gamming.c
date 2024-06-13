#include "event_handler.h"
#include "gamming.h"

int search_event( GameState *game_state ) {
    int is_find = 0;
    for(int i = 0; i < event_count; i++) {
        if(strcmp(game_state->event, events[i].id) == 0) {
            is_find = 1;
            for(int j = 0; j < character_count; j++ ) {
                if(strcmp(events[i].character, characters[j].id) == 0) {
                    // strcpy(game_state->character, characters[j].sprite);
                    game_state->character = characters[j].sprite;
                    break;
                }
            }
            for(int j = 0; j < scene_count; j++) {
                if(strcmp(events[j].scene, scenes[j].id) == 0) {
                    // strcpy(game_state->scene, scenes[j].background);
                    game_state->scene = scenes[j].background;
                    break;
                }
            }
            strcpy(game_state->dialogue_text, events[i].dialogue);
            if(events[i].choice_count > 0) {
                game_state->have_choice = 1;
                // strcpy(game_state->choice_a, events[i].choices[0].text);
                // strcpy(game_state->choice_b, events[i].choices[1].text);
                // strcpy(game_state->choice_c, events[i].choices[2].text);
                // strcpy(game_state->option1_event, events[i].choices[0].next_event);
                // strcpy(game_state->option2_event, events[i].choices[1].next_event);
                // strcpy(game_state->option3_event, events[i].choices[2].next_event);
                game_state->choice_a = events[i].choices[0].text;
                game_state->choice_b = events[i].choices[1].text;
                game_state->choice_c = events[i].choices[2].text;
                game_state->option1_event = events[i].choices[0].next_event;
                game_state->option2_event = events[i].choices[1].next_event;
                game_state->option3_event = events[i].choices[2].next_event;
                // game_state->next_event = "0";
            }
            else {
                // strcpy(game_state->next_event, events[i].next_event);
                game_state->next_event = events[i].next_event;
                // game_state->choice_a = "0";
                // game_state->choice_b = "0";
                // game_state->choice_c = "0";
                // game_state->option1_event = "0";
                // game_state->option2_event = "0";
                // game_state->option3_event = "0";
                game_state->have_choice = 0;
            }
            game_state->affect_change = 50;

            break;
        }
    }
    if( is_find )
        return 0;
    else
        return 1;
}
