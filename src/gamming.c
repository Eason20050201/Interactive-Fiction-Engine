#include "event_handler.h"
#include "gamming.h"

char *save_event_id;

void search_event( GameState *game_state ) {
    for(int i = 0; i < event_count; i++) {
        if(strcmp(game_state->event, events[i].id) == 0) {
            save_event_id = strdup(events[i].id);
            for(int j = 0; j < character_count; j++ ) {
                if(strcmp(events[i].character, characters[j].id) == 0) {
                    game_state->character = strdup(characters[j].sprite);
                    game_state->character_name = strdup(characters[j].name);
                    break;
                }
            }
            for(int j = 0; j < scene_count; j++) {
                if(strcmp(events[i].scene, scenes[j].id) == 0) {
                    game_state->scene = strdup(scenes[j].background);
                    break;
                }
            }
            game_state->dialogue_text = strdup(events[i].dialogue);
            if(events[i].choice_count > 0) {
                game_state->have_choice = 1;
                game_state->choice_a = strdup(events[i].choices[0].text);
                game_state->choice_b = strdup(events[i].choices[1].text);
                game_state->choice_c = strdup(events[i].choices[2].text);
                game_state->option1_event = strdup(events[i].choices[0].next_event);
                game_state->option2_event = strdup(events[i].choices[1].next_event);
                game_state->option3_event = strdup(events[i].choices[2].next_event);
                if(events[i].choices[0].required > 0) {
                    game_state->option1_required = events[i].choices[0].required;
                    game_state->option1_required_id = strdup(events[i].choices[0].required_id);
                }
                if(events[i].choices[1].required > 0) {
                    game_state->option2_required = events[i].choices[1].required;
                    game_state->option2_required_id = strdup(events[i].choices[1].required_id);
                }
                if(events[i].choices[2].required > 0) {
                    game_state->option3_required = events[i].choices[2].required;
                    game_state->option3_required_id = strdup(events[i].choices[2].required_id);
                }
                if(events[i].choices[0].affection_changes != 0) {
                    game_state->option1_affection_change = events[i].choices[0].affection_changes;
                    game_state->option1_character_id = strdup(events[i].choices[0].character_id);
                }
                if(events[i].choices[1].affection_changes != 0) {
                    game_state->option2_affection_change = events[i].choices[1].affection_changes;
                    game_state->option2_character_id = strdup(events[i].choices[1].character_id);
                }
                if(events[i].choices[2].affection_changes != 0) {
                    game_state->option3_affection_change = events[i].choices[2].affection_changes;
                    game_state->option3_character_id = strdup(events[i].choices[2].character_id);
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
                        game_state->next_event = strdup(events[i].judge_event[j].next_event);
                        game_state->have_choice = 0;
                        break;
                    }
                }
            }
            else {
                game_state->next_event = strdup(events[i].next_event);
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

void save_game(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open file for writing");
        return;
    }

    // 儲存事件 ID
    fprintf(file, "event_id: %s\n", save_event_id);

    // 儲存角色資料
    fprintf(file, "characters:\n");
    for (int i = 0; i < character_count; i++) {
        fprintf(file, "  - id: %s\n    affection: %d\n", characters[i].id, characters[i].affection);
    }

    // 儲存物品資料
    fprintf(file, "items:\n");
    for (int i = 0; i < item_count; i++) {
        fprintf(file, "  - id: %s\n    quantity: %d\n", items[i].id, items[i].quantity);
    }

    fclose(file);
}

void load_game(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file for reading");
        return;
    }

    char line[256];
    int save_character_count = 0;
    int save_item_count = 0;
    int in_character = 0;
    int in_item = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "event_id:", 9) == 0) {
            save_event_id = strdup(line + 10);
            save_event_id[strcspn(save_event_id, "\n")] = '\0'; // 移除換行符號
        }
        else if (strncmp(line, "characters:", 11) == 0) {
            in_character = 1;
        }
        else if (in_character == 1) {
            if(save_character_count < character_count && strstr(line, "  - id: ")) {
                characters[save_character_count].id = strdup(line + 8);
                characters[save_character_count].id[strcspn(characters[save_character_count].id, "\n")] = '\0'; // 移除換行符號
                fgets(line, sizeof(line), file);
                sscanf(line, "    affection: %d", &characters[save_character_count].affection);
                save_character_count++;
            }
            if(save_character_count == character_count) {
                in_character = 0;
            }
        }
        else if (strncmp(line, "items:", 6) == 0) {
            in_item = 1;
        }
        else if (in_item == 1) {
            if (save_item_count < item_count && strstr(line, "  - id: ")) {
                items[save_item_count].id = strdup(line + 8);
                items[save_item_count].id[strcspn(items[save_item_count].id, "\n")] = '\0'; // 移除換行符號
                fgets(line, sizeof(line), file);
                sscanf(line, "    quantity: %d", &items[save_item_count].quantity);
                save_item_count++;
            }
            if (save_item_count == item_count) {
                in_item = 0;
            }
        }
    }

    fclose(file);
}