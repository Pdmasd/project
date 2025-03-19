// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "graphic.h"

struct Player {
    int x, y; // Vị trí nhân vật
    int speed; // Tốc độ di chuyển
    int velocityX, velocityY; // Vận tốc di chuyển
    SDL_Texture* texture;

    void init(Graphic& graphic, const char* filename, int startX, int startY, int moveSpeed);
    void handleEvent(SDL_Event& e);
    void update();
    void render(Graphic& graphic);
    void cleanUp();
};

#endif

