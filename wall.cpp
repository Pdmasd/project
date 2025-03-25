//wall.cpp
#include "wall.h"

Wall::Wall(int startX, int startY, SDL_Renderer* renderer, const char* texturePath, WallType type)
    : x(startX), y(startY), active(true), type(type) { // Đảm bảo gán đúng
    rect = {x, y, TILE_SIZE, TILE_SIZE};

    SDL_Surface* surface = IMG_Load(texturePath);
    if (!surface) {
        SDL_Log("Không thể load ảnh: %s, Lỗi: %s", texturePath, IMG_GetError());
        texture = nullptr;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

// Destructor: Giải phóng texture
Wall::~Wall() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

// Move constructor
Wall::Wall(Wall&& other) noexcept
    : x(other.x), y(other.y), rect(other.rect), texture(other.texture),
      active(other.active), type(other.type) {
    other.texture = nullptr;
}

// Move assignment operator
Wall& Wall::operator=(Wall&& other) noexcept {
    if (this != &other) {
        if (texture) SDL_DestroyTexture(texture);

        x = other.x;
        y = other.y;
        rect = other.rect;
        texture = other.texture;
        active = other.active;
        type = other.type;

        other.texture = nullptr;
    }
    return *this;
}

void Wall::render(SDL_Renderer* renderer) {
    if (active && texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

bool Wall::isActive() const {
    return active;
}


