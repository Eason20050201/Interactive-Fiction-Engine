#include "event_handler.h"
#include "gamming.h"

void search_event( GameState *game_state ) {
    for(int i = 0; i < event_count; i++) {
        if(strcmp(game_state->event, events[i].id) == 0) {
            for(int j = 0; j < character_count; j++ ) {
                if(strcmp(events[i].character, characters[j].id) == 0) {
                    strcpy(game_state->character, characters[j].sprite);
                    strcpy(game_state->character_name, characters[j].name);
                    break;
                }
            }
            for(int j = 0; j < scene_count; j++) {
                if(strcmp(events[i].scene, scenes[j].id) == 0) {
                    strcpy(game_state->scene, scenes[j].background);
                    break;
                }
            }
            strcpy(game_state->dialogue_text, events[i].dialogue);
            if(events[i].choice_count > 0) {
                game_state->have_choice = 1;
                strcpy(game_state->choice_a, events[i].choices[0].text);
                strcpy(game_state->choice_b, events[i].choices[1].text);
                strcpy(game_state->choice_c, events[i].choices[2].text);
                strcpy(game_state->option1_event, events[i].choices[0].next_event);
                strcpy(game_state->option2_event, events[i].choices[1].next_event);
                strcpy(game_state->option3_event, events[i].choices[2].next_event);

                if(events[i].choices[0].required > 0) {
                    game_state->option1_required = events[i].choices[0].required;
                    strcpy(game_state->option1_required_id, events[i].choices[0].required_id);
                }
                if(events[i].choices[1].required > 0) {
                    game_state->option2_required = events[i].choices[1].required;
                    strcpy(game_state->option2_required_id, events[i].choices[1].required_id);
                }
                if(events[i].choices[2].required > 0) {
                    game_state->option3_required = events[i].choices[2].required;
                    strcpy(game_state->option3_required_id, events[i].choices[2].required_id);
                }
            }
            else if(events[i].judge_event_count > 0) {
                int character_id_index[events[i].judge_event_count];
                for(int j = 0; j < events[i].judge_event_count; j++) {
                    for(int k = j + 1; k < events[i].judge_event_count; k++) {
                        if(events[i].judge_event[j].required_affection < events[i].judge_event[k].required_affection)
                        {
                            Judge_Event temp = events[i].judge_event[j];
                            events[i].judge_event[j] = events[i].judge_event[k];
                            events[i].judge_event[k] = temp;
                        }
                    }
                    for(int k = 0; k < character_count; k++) {
                        if(strcmp(events[i].judge_event[j].character_id, characters[k].id) == 0) {
                            character_id_index[j] = k;
                            break;
                        }
                    }
                }
                for(int j = 0; j < events[i].judge_event_count; j++ ) {
                    if( characters[character_id_index[j]].affection >= events[i].judge_event[j].required_affection)
                    {
                        strcpy(game_state->next_event, events[i].judge_event[j].next_event);
                        game_state->have_choice = 0;
                        break;
                    }
                }
            }
            else {
                strcpy(game_state->next_event, events[i].next_event);
                game_state->have_choice = 0;
            }

            
            if(events[i].obtain > 0) {
                for(int j = 0; j < item_count; j++) {
                    if(strcmp(events[i].obtain_id, items[j].id) == 0) {
                        items[j].quantity += events[i].obtain;
                        break;
                    }
                }
            }

            break;
        }
    }

}
