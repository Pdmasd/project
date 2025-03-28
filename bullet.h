//bullet.h
#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

#include "defs.h"
#include "wall.h"

class Bullet {
public:
    int x, y;
    int dx, dy;
    SDL_Rect rect;
    bool active;

    bool was_moved = false;

    Bullet (int startX, int startY, int dirX, int dirY);
    void move();
    void render(SDL_Renderer* renderer);

    // Kiểm tra va chạm với Wall
    bool checkCollision(const Wall& wall) const;
};

#endif
