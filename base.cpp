// base.cpp
#include "base.h"

Base::Base(int startX, int startY, SDL_Renderer* renderer, const char* texturePath)
    : x(startX), y(startY), destroyed(false) {
    rect = {x, y, TILE_SIZE, TILE_SIZE};

    SDL_Surface* surface = IMG_Load(texturePath);
    if (!surface) {
        SDL_Log("Không thể tải ảnh nhà chính: %s", IMG_GetError());
        texture = nullptr;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

Base::~Base() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Base::render(SDL_Renderer* renderer) {
    if (!destroyed && texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

bool Base::isDestroyed() const {
    return destroyed;
}

void Base::destroy() {
    destroyed = true;
}

