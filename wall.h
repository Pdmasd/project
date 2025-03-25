//wall.h
#ifndef WALL_H
#define WALL_H

#include <SDL.h>
#include "defs.h"

class Wall {
public:
    int x, y;
    bool active;
    SDL_Rect rect;
    SDL_Texture* texture;

    Wall(int startX, int startY, SDL_Renderer* renderer, const char* texturePath);

    // Destructor: Giải phóng texture
    ~Wall();

    // Hiển thị Wall
    void render(SDL_Renderer* renderer);

    // Kiểm tra trạng thái
    bool isActive() const { return active; }

    Wall(Wall&& other) noexcept;
    Wall& operator=(Wall&& other) noexcept;
};

#endif


