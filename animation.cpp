//animation.cpp
#include "animation.h"

Animation::Animation() : texture(nullptr), currentFrame(0), lastFrameTime(0), frameDelay(100) {}

Animation::Animation(SDL_Texture* texture, int frameWidth, int frameHeight, int frameDelay)
    : texture(texture), currentFrame(0), lastFrameTime(0), frameDelay(frameDelay) {}

Animation::~Animation() {}

// Thêm frame từ sprite sheet
void Animation::addFrame(int x, int y) {
    SDL_Rect frame = { x, y, 16, 16 }; // Mỗi frame là 16x16
    frames.push_back(frame);
}

// Cập nhật frame theo thời gian
void Animation::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime >= frameDelay) {
        currentFrame = (currentFrame + 1) % frames.size();
        lastFrameTime = currentTime;
    }
}

// Vẽ frame hiện tại
void Animation::render(SDL_Renderer* renderer, int x, int y) {
    if (texture && !frames.empty()) {
        SDL_Rect srcRect = frames[currentFrame]; // Frame hiện tại (16x16)

        // Tạo kích thước mới để vẽ ra màn hình
        SDL_Rect destRect = {x, y, TILE_SIZE, TILE_SIZE};

        // Vẽ frame hiện tại với kích thước TILE_SIZE
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }
}

void Animation::reset() {
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
}
