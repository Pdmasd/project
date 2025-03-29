//animation.cpp
#include "animation.h"

Animation::Animation()
    : texture(nullptr), currentFrame(0), lastFrameTime(0), frameDelay(100),
      looping(true), finished(false){}

Animation::Animation(SDL_Texture* texture, int frameWidth, int frameHeight, int frameDelay)
    : texture(texture), currentFrame(0), lastFrameTime(0), frameDelay(frameDelay),
      looping(true), finished(false) {}

Animation::~Animation() {}

void Animation::addFrame(int x, int y) {
    SDL_Rect frame = { x, y, 16, 16 };  /// Mỗi frame là 16x16
    frames.push_back(frame);
}

void Animation::update() {
    if (frames.empty() || finished)
        return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime >= static_cast<Uint32>(frameDelay)) {
        currentFrame++;
        lastFrameTime = currentTime;
        if (currentFrame >= static_cast<int>(frames.size())) {
            if (looping) {
                currentFrame = 0;
            } else {
                /// Giữ frame cuối và đánh dấu đã hoàn thành.
                currentFrame = frames.size() - 1;
                finished = true;
            }
        }
    }
}

void Animation::render(SDL_Renderer* renderer, int x, int y) {
    if (texture && !frames.empty()) {
        SDL_Rect srcRect = frames[currentFrame];

        /// Tạo kích thước mới để vẽ ra màn hình
        SDL_Rect destRect = {x, y, TILE_SIZE, TILE_SIZE};

        /// Vẽ frame hiện tại với kích thước TILE_SIZE
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }
}

void Animation::reset() {
    currentFrame = 0;
    lastFrameTime = SDL_GetTicks();
    finished = false;
}

bool Animation::isFinished() const {
    return finished;
}

void Animation::setLooping(bool looping) {
    this->looping = looping;
}
