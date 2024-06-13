#ifndef PARSER_H
#define PARSER_H

#include "toml.h"

typedef struct {
    char* id;
    char* background;
    char* description;
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
    int quantity;   // 物品數量
} Item;

typedef struct {
    char* text;
    char* next_event;
    char* character_id;
    char* required_id;
    char* optain_id;
    int required;
    int optain;
    int affection_change;
} Choice;

typedef struct {
    char* id;
    char* scene;
    char* character;
    char* dialogue;
    char* next_event;
    Choice* choices;
    int choice_count;
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