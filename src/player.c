#include "player.h"
#include "graphics.h"

void set_player_name(SDL_Renderer *renderer, char *player_name) {
    // Initialize font
    TTF_Font *font = TTF_OpenFont("example-game/assets/NotoSansCJKtc-Regular.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    char inputText[256] = "Enter your name: ";
    SDL_StartTextInput();

    SDL_Event e;
    int quit = 0;
    int inputComplete = 0;
    while (!quit && !inputComplete) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_TEXTINPUT) {
                strcat(inputText, e.text.text);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > strlen("Enter your name: ")) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    sscanf(inputText, "Enter your name: %s", player_name); // 将输入文本复制到 player_name
                    printf("User input: %s\n", player_name);
                    inputComplete = 1; // 停止输入
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
        SDL_RenderClear(renderer);

        render_text(renderer, inputText, 200, 200, 400, 50, 255, 255, 255);

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
