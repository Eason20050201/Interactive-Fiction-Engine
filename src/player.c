#include "player.h"
#include "graphics.h"
#include "macro.h"
#include <ctype.h>

// 计算UTF-8字符串的字符数
int utf8_strlen(const char *str) {
    int count = 0;
    const char *s = str;
    while (*s) {
        if ((*s & 0xc0) != 0x80) {
            count++;
        }
        s++;
    }
    return count;
}

// 检查字符串是否为空或仅包含空白字符
int is_empty_or_whitespace(const char *str) {
    const char *s = str;
    while (*s) {
        if (!isspace((unsigned char)*s)) {
            return 0; // 字符串包含非空白字符
        }
        s++;
    }
    return 1; // 字符串为空或仅包含空白字符
}

void set_player_name(SDL_Renderer *renderer, char *player_name, int *running) {
    // Initialize font
    TTF_Font *font = TTF_OpenFont("example-game/assets/NotoSansCJKtc-Regular.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    const char *prompt = "請輸入你的名字（小於五個字）: ";
    char inputText[256];
    strcpy(inputText, prompt);
    SDL_StartTextInput();

    SDL_Event e;
    int quit = 0;
    int inputComplete = 0;
    while (!quit && !inputComplete) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
                *running = 0; // 终止程序运行
            } else if (e.type == SDL_TEXTINPUT) {
                // 计算实际输入的字符数
                if (utf8_strlen(inputText) < utf8_strlen(prompt) + 5) {  // 5 characters limit
                    strcat(inputText, e.text.text);
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > strlen(prompt)) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    char tempName[256] = ""; // 清空临时变量
                    sscanf(inputText + strlen(prompt), "%255s", tempName); // 将输入文本复制到临时变量，限制最大长度

                    // 检查是否为空或仅包含空白字符
                    if (is_empty_or_whitespace(tempName)) {
                        strcpy(player_name, "纪博文"); // 设置默认名称
                    } else {
                        strcpy(player_name, tempName); // 使用用户输入的名称
                    }

                    printf("User input: %s\n", player_name);
                    inputComplete = 1; // 停止输入
                }
            }
        }

        SDL_RenderClear(renderer);

        SDL_Texture *texture = load_texture("enter_name.png", renderer);
        render_texture_fullscreen(texture, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
        render_name_box(renderer, inputText, 300, 300, 700, 50);

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_RenderClear(renderer);  // Clear the renderer before drawing new content
}

void replaceSubstring(char *source, const char *target, const char *replacement) {
    char buffer[1024]; // 临时缓冲区
    char *insertPoint = &buffer[0];
    const char *temp = source;
    size_t targetLen = strlen(target);
    size_t replacementLen = strlen(replacement);

    while (1) {
        const char *p = strstr(temp, target);

        // 如果找不到目标子字符串
        if (p == NULL) {
            strcpy(insertPoint, temp);
            break;
        }

        // 复制目标子字符串之前的部分
        memcpy(insertPoint, temp, p - temp);
        insertPoint += p - temp;

        // 复制替换字符串
        memcpy(insertPoint, replacement, replacementLen);
        insertPoint += replacementLen;

        // 移动到目标子字符串之后
        temp = p + targetLen;
    }

    // 将结果复制回源字符串
    strcpy(source, buffer);
}
