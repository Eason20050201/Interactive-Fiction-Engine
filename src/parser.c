#include <stdio.h>   // 包含標準輸入輸出頭文件，用於打印錯誤信息等
#include <stdlib.h>  // 包含標準庫頭文件，用於動態內存分配等
#include <string.h>  // 包含字符串操作頭文件，用於字符串操作
#include "parser.h"  // 包含自定義的頭文件，用於解析TOML

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

// 解析TOML文件的函數
int parse_toml(const char* filename) {
    char error[1024] = {0};
    FILE* fp = fopen(filename, "r"); // 打開文件
    if (!fp) {
        printf("Failed to open %s\n", filename); // 如果打開文件失敗，打印錯誤信息
        return 1;
    }

    toml_table_t* conf = toml_parse_file(fp, error, 0); // 解析TOML文件
    fclose(fp); // 關閉文件
    if (!conf) {
        printf("Failed to parse %s\n", filename); // 如果解析失敗，打印錯誤信息
        return 1;
    }

    parse_scenes(conf); // 解析場景
    parse_characters(conf); // 解析角色
    parse_items(conf); // 解析物品
    parse_events(conf); // 解析事件

    toml_free(conf); // 釋放TOML表
    return 0; // 返回成功
}

// 解析場景的函數
void parse_scenes(toml_table_t* conf) {
    toml_array_t* scenes_arr = toml_array_in(conf, "scenes"); // 獲取場景數組
    scene_count = toml_array_nelem(scenes_arr); // 獲取場景數量
    scenes = malloc(scene_count * sizeof(Scene)); // 分配內存

    for (int i = 0; i < scene_count; i++) {
        toml_table_t* scene = toml_table_at(scenes_arr, i); // 獲取每個場景
        char* id;
        char* background;
        char* description;

        // 獲取場景的ID、背景和描述
        if (toml_rtos(toml_raw_in(scene, "id"), &id) == 0 &&
            toml_rtos(toml_raw_in(scene, "background"), &background) == 0 &&
            toml_rtos(toml_raw_in(scene, "description"), &description) == 0) {
            scenes[i].id = strdup(id); // 複製ID
            scenes[i].background = strdup(background); // 複製背景
            scenes[i].description = strdup(description); // 複製描述
        }
    }
}

// 解析角色的函數
void parse_characters(toml_table_t* conf) {
    toml_array_t* characters_arr = toml_array_in(conf, "characters"); // 獲取角色數組
    character_count = toml_array_nelem(characters_arr); // 獲取角色數量
    characters = malloc(character_count * sizeof(Character)); // 分配內存

    for (int i = 0; i < character_count; i++) {
        toml_table_t* character = toml_table_at(characters_arr, i); // 獲取每個角色
        char* id;
        char* name;
        char* avatar;
        char* sprite;
        int64_t affection;

        // 獲取角色的ID、名字、頭像、立繪和好感度
        if (toml_rtos(toml_raw_in(character, "id"), &id) == 0 &&
            toml_rtos(toml_raw_in(character, "name"), &name) == 0 &&
            toml_rtos(toml_raw_in(character, "avatar"), &avatar) == 0 &&
            toml_rtos(toml_raw_in(character, "sprite"), &sprite) == 0 &&
            toml_rtoi(toml_raw_in(character, "affection"), &affection) == 0) {
            characters[i].id = strdup(id); // 複製ID
            characters[i].name = strdup(name); // 複製名字
            characters[i].avatar = strdup(avatar); // 複製頭像
            characters[i].sprite = strdup(sprite); // 複製立繪
            characters[i].affection = affection; // 設置好感度
        }
    }
}

// 解析物品的函數
void parse_items(toml_table_t* conf) {
    toml_array_t* items_arr = toml_array_in(conf, "items"); // 獲取物品數組
    item_count = toml_array_nelem(items_arr); // 獲取物品數量
    items = malloc(item_count * sizeof(Item)); // 分配內存

    for (int i = 0; i < item_count; i++) {
        toml_table_t* item = toml_table_at(items_arr, i); // 獲取每個物品
        char* id;
        char* name;
        char* icon;
        int64_t quantity; // 新增數量字段

        // 獲取物品的ID、名字、圖標和數量
        if (toml_rtos(toml_raw_in(item, "id"), &id) == 0 &&
            toml_rtos(toml_raw_in(item, "name"), &name) == 0 &&
            toml_rtos(toml_raw_in(item, "icon"), &icon) == 0 &&
            toml_rtoi(toml_raw_in(item, "quantity"), &quantity) == 0) {
            items[i].id = strdup(id); // 複製ID
            items[i].name = strdup(name); // 複製名字
            items[i].icon = strdup(icon); // 複製圖標
            items[i].quantity = quantity; // 設置數量
        } else {
            items[i].quantity = 1; // 如果未設置數量，默認為1
        }
    }
}

// 解析事件的函數
void parse_events(toml_table_t* conf) {
    toml_array_t* events_arr = toml_array_in(conf, "events"); // 獲取事件數組
    event_count = toml_array_nelem(events_arr); // 獲取事件數量
    events = malloc(event_count * sizeof(Event)); // 分配內存

    for (int i = 0; i < event_count; i++) {
        toml_table_t* event = toml_table_at(events_arr, i); // 獲取每個事件
        char* scene;
        char* character;
        char* dialogue;

        // 獲取事件的場景、角色和對話
        if (toml_rtos(toml_raw_in(event, "scene"), &scene) == 0 &&
            toml_rtos(toml_raw_in(event, "character"), &character) == 0 &&
            toml_rtos(toml_raw_in(event, "dialogue"), &dialogue) == 0) {
            events[i].scene = strdup(scene); // 複製場景
            events[i].character = strdup(character); // 複製角色
            events[i].dialogue = strdup(dialogue); // 複製對話

            parse_choices(event, &events[i]); // 解析選擇
        }
    }
}

// 解析選擇的函數
void parse_choices(toml_table_t* event, Event* evt) {
    toml_array_t* choices_arr = toml_array_in(event, "choices"); // 獲取選擇數組
    evt->choice_count = toml_array_nelem(choices_arr); // 獲取選擇數量
    evt->choices = malloc(evt->choice_count * sizeof(Choice)); // 分配內存

    for (int j = 0; j < evt->choice_count; j++) {
        toml_table_t* choice = toml_table_at(choices_arr, j); // 獲取每個選擇
        char* text;
        char* next_event;
        int64_t affection_change = 0; // 默認為0，如果沒有該字段

        // 獲取選擇的文本、下一事件和好感度變化
        if (toml_rtos(toml_raw_in(choice, "text"), &text) == 0 &&
            toml_rtos(toml_raw_in(choice, "next_event"), &next_event) == 0) {
            evt->choices[j].text = strdup(text); // 複製文本
            evt->choices[j].next_event = strdup(next_event); // 複製下一事件
            if (toml_rtoi(toml_raw_in(choice, "affection_change"), &affection_change) == 0) {
                evt->choices[j].affection_change = affection_change; // 設置好感度變化
            } else {
                evt->choices[j].affection_change = 0; // 如果沒有設置好感度變化，默認0
            }
        }
    }
}
