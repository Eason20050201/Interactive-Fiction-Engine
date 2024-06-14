#ifndef PARSER_H
#define PARSER_H

#include "toml.h"

typedef struct {
    char* id;
    char* background;
} Scene;

typedef struct {
    char* id;
    char* name;
    char* avatar;
    char* sprite;
    int affection;
} Character;

typedef struct {
    char* id;
    char* name;
    char* icon;
    char* description;
    int quantity;   // 物品數量
} Item;

typedef struct {
    char* text;
    char* next_event;
    char* character_id;
    char* required_id;
    int required;
    int affection_changes;
} Choice;

typedef struct {
    char* character_id;
    int required_affection;
    char* next_event;
}Judge_Event;

typedef struct {
    char* id;
    char* scene;
    char* character;
    char* dialogue;
    char* next_event;
    char* obtain_id;
    int obtain;
    Choice* choices;
    Judge_Event* judge_event;
    int choice_count;
    int judge_event_count;
} Event;

extern Scene* scenes;
extern int scene_count;
extern Character* characters;
extern int character_count;
extern Item* items;
extern int item_count;
extern Event* events;
extern int event_count;

int parse_toml(const char* filename);

#endif
