#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// 定義全局變數來存儲解析的數據
Scene* scenes;
int scene_count;
Character* characters;
int character_count;
Item* items;
int item_count;
Event* events;
int event_count;

// 函數原型
void parse_scenes(toml_table_t* conf);
void parse_characters(toml_table_t* conf);
void parse_items(toml_table_t* conf);
void parse_events(toml_table_t* conf);
void parse_choices(toml_table_t* event, Event* evt);
void parse_judge_event(toml_table_t* event, Event* evt);
void update_affection(const char* character_id, int affection_change);
void handle_choice(Event* evt, int choice_index);

// 解析TOML文件的函數
int parse_toml(const char* filename) {
    char error[1024] = {0};
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        return 1;
    }

    toml_table_t* conf = toml_parse_file(fp, error, 0);
    fclose(fp);
    if (!conf) {
        printf("Failed to parse %s\n", filename);
        return 1;
    }

    parse_scenes(conf);
    parse_characters(conf);
    parse_items(conf);
    parse_events(conf);

    toml_free(conf);
    return 0;
}

void parse_scenes(toml_table_t* conf) {
    toml_array_t* scenes_arr = toml_array_in(conf, "scenes");
    scene_count = toml_array_nelem(scenes_arr);
    scenes = malloc(scene_count * sizeof(Scene));

    for (int i = 0; i < scene_count; i++) {
        toml_table_t* scene = toml_table_at(scenes_arr, i);
        char* id;
        char* background;

        if (toml_rtos(toml_raw_in(scene, "id"), &id) == 0 &&
            toml_rtos(toml_raw_in(scene, "background"), &background) == 0) {
            scenes[i].id = strdup(id);
            scenes[i].background = strdup(background);
        }
    }
}

void parse_characters(toml_table_t* conf) {
    toml_array_t* characters_arr = toml_array_in(conf, "characters");
    character_count = toml_array_nelem(characters_arr);
    characters = malloc(character_count * sizeof(Character));

    for (int i = 0; i < character_count; i++) {
        toml_table_t* character = toml_table_at(characters_arr, i);
        char* id;
        char* name;
        char* avatar;
        char* sprite;
        int64_t affection;

        if (toml_rtos(toml_raw_in(character, "id"), &id) == 0 &&
            toml_rtos(toml_raw_in(character, "name"), &name) == 0 &&
            toml_rtos(toml_raw_in(character, "avatar"), &avatar) == 0 &&
            toml_rtos(toml_raw_in(character, "sprite"), &sprite) == 0 &&
            toml_rtoi(toml_raw_in(character, "affection"), &affection) == 0) {
            characters[i].id = strdup(id);
            characters[i].name = strdup(name);
            characters[i].avatar = strdup(avatar);
            characters[i].sprite = strdup(sprite);
            characters[i].affection = affection;
        }
    }
}

void parse_items(toml_table_t* conf) {
    toml_array_t* items_arr = toml_array_in(conf, "items");
    item_count = toml_array_nelem(items_arr);
    items = malloc(item_count * sizeof(Item));

    for (int i = 0; i < item_count; i++) {
        toml_table_t* item = toml_table_at(items_arr, i);
        char* id;
        char* name;
        char* icon;
        int64_t quantity;

        if (toml_rtos(toml_raw_in(item, "id"), &id) == 0 &&
            toml_rtos(toml_raw_in(item, "name"), &name) == 0 &&
            toml_rtos(toml_raw_in(item, "icon"), &icon) == 0 &&
            toml_rtoi(toml_raw_in(item, "quantity"), &quantity) == 0) {
            items[i].id = strdup(id);
            items[i].name = strdup(name);
            items[i].icon = strdup(icon);
            items[i].quantity = quantity;
        } else {
            items[i].quantity = 0;
        }
    }
}

void parse_events(toml_table_t* conf) {
    toml_array_t* events_arr = toml_array_in(conf, "events");
    event_count = toml_array_nelem(events_arr);
    events = malloc(event_count * sizeof(Event));

    for (int i = 0; i < event_count; i++) {
        toml_table_t* event = toml_table_at(events_arr, i);
        char* scene = NULL;
        char* character = NULL;
        char* dialogue = NULL;
        char* next_event = NULL;
        char* id = NULL;
        char* obtain_id = NULL;
        int64_t obtain = 0;
        events[i].obtain_id = NULL;
        events[i].obtain = 0;
        events[i].choice_count = 0;
        events[i].judge_event_count = 0;
        events[i].next_event = NULL;

        if (toml_rtos(toml_raw_in(event, "scene"), &scene) == 0 &&
            toml_rtos(toml_raw_in(event, "character"), &character) == 0 &&
            toml_rtos(toml_raw_in(event, "dialogue"), &dialogue) == 0 &&
            toml_rtos(toml_raw_in(event, "id"), &id) == 0){
            events[i].scene = strdup(scene);
            events[i].character = strdup(character);
            events[i].dialogue = strdup(dialogue);
            events[i].id = strdup(id);
            if (toml_rtos(toml_raw_in(event, "obtain_id"), &obtain_id) == 0 &&
                toml_rtoi(toml_raw_in(event, "obtain"), &obtain) == 0){
                events[i].obtain_id = strdup(obtain_id);
                events[i].obtain = obtain;
            }
            if(toml_rtos(toml_raw_in(event, "next_event"), &next_event) == 0){
                events[i].next_event = strdup(next_event);
            }
            else if(strstr(events[i].id, "JUDGE") != NULL){
                parse_judge_event(event, &events[i]);
            }
            else{
                parse_choices(event, &events[i]);
            }
            
        }
    }
}

void parse_judge_event(toml_table_t* event, Event* evt){
    toml_array_t* judge_event_arr = toml_array_in(event, "judge_event");
    evt->judge_event_count = toml_array_nelem(judge_event_arr);
    evt->judge_event = malloc(evt->judge_event_count * sizeof(Judge_Event));

    for (int j = 0; j < evt->judge_event_count; j++) {
        toml_table_t* judge_event = toml_table_at(judge_event_arr, j);
        char* next_event = NULL;
        char* character_id = NULL;
        int64_t required_affection = 0;
        if (toml_rtos(toml_raw_in(judge_event, "character_id"), &character_id) == 0 &&
            toml_rtos(toml_raw_in(judge_event, "next_event"), &next_event) == 0 && 
            toml_rtoi(toml_raw_in(judge_event, "required_affection"), &required_affection) == 0) {
            evt->judge_event[j].character_id = strdup(character_id);
            evt->judge_event[j].next_event = strdup(next_event);
            evt->judge_event[j].required_affection = required_affection;
        }
    }
}

void parse_choices(toml_table_t* event, Event* evt) {
    toml_array_t* choices_arr = toml_array_in(event, "choices");
    evt->choice_count = toml_array_nelem(choices_arr);
    evt->choices = malloc(evt->choice_count * sizeof(Choice));

    for (int j = 0; j < evt->choice_count; j++) {
        toml_table_t* choice = toml_table_at(choices_arr, j);
        char* text;
        char* next_event = NULL;
        char* character_id = NULL;
        char* required_id = NULL;
        int64_t affection_change = 0;
        int64_t required = 0;

        if (toml_rtos(toml_raw_in(choice, "text"), &text) == 0 &&
            toml_rtos(toml_raw_in(choice, "next_event"), &next_event) == 0) {
            evt->choices[j].text = strdup(text);
            evt->choices[j].next_event = strdup(next_event);
            if (toml_rtos(toml_raw_in(choice, "character_id"), &character_id) == 0) {
                evt->choices[j].character_id = strdup(character_id);
            } else {
                evt->choices[j].character_id = NULL;
            }
            if (toml_rtoi(toml_raw_in(choice, "affection_change"), &affection_change) == 0) {
                evt->choices[j].affection_change = affection_change;
            } else {
                evt->choices[j].affection_change = 0;
            }
            if (toml_rtoi(toml_raw_in(choice, "required"), &required) == 0) {
                evt->choices[j].required = required;
            } else {
                evt->choices[j].required = 0;
            }
            if (toml_rtos(toml_raw_in(choice, "required_id"), &required_id) == 0) {
                evt->choices[j].required_id = strdup(required_id);
            } else {
                evt->choices[j].required_id = NULL;
            }
        }
    }
}
