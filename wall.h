//wall.h
#ifndef WALL_H
#define WALL_H

#include <SDL.h>
#include "defs.h"

// Loại khối
enum class WallType {
    BRICK,      /// Khối gạch (có thể bị phá hủy)
    STEEL,      /// Khối thép (không thể bị phá hủy)
    LEAF,       /// Khối lá (có thể đi và bắn qua)
    WATER       /// Khối nước (không thể đi qua nhưng bắn qua được)
};

class Wall {
public:
    int x, y;
    bool active;
    SDL_Rect rect;
    SDL_Texture* texture;
    WallType type; // Loại khối

    Wall(int startX, int startY, SDL_Renderer* renderer, const char* texturePath, WallType type);
    ~Wall();


    void render(SDL_Renderer* renderer);
    bool isActive() const;

    WallType getType() const { return type; }

    Wall(Wall&& other) noexcept;
    Wall& operator=(Wall&& other) noexcept;

    static std::vector<Wall*> createBrickGroup(int startX, int startY, SDL_Renderer* renderer, const char* texturePath);
    void hit();
};

#endif


