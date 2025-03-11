#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL.h>
#include <SDL_image.h>

struct Graphic{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    void logErrorAndExit(const char* msg, const char* error);
    void initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE);
    void quitSDL();
    SDL_Texture *loadTexture(const char *filename);
    void renderTexture(SDL_Texture *texture, int x = 0, int y = 0, int w = 100, int h = 100);
        // chuẩn bị renderer
    void prepareScene();
        // hiển thị renderer ra màn hình
    void presentScene();
};
#endif
