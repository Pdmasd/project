#include "explosion.h"

Explosion::Explosion(SDL_Texture* texture, int x, int y, int explosion_type)
    : texture(texture), currentFrame(0), frameTime(300), elapsedTime(0), finished(false)
{

    position.x = x;
    position.y = y;
    if(explosion_type == 0 || explosion_type == 1){            ///Nổ loại to
        position.w = TILE_SIZE;
        position.h = TILE_SIZE;
    } else if(explosion_type == 2){     ///Nổ loại nhỏ
        position.w = TILE_SIZE / 2;
        position.h = TILE_SIZE / 2;
    }
    initFrames(explosion_type);
}

Explosion::~Explosion() {
}

void Explosion::initFrames(int explosion_type) {
    SDL_Rect frame;
    if(explosion_type == 0){
        frame = {304, 128, 32, 32};
        frames.push_back(frame);

        frame = {336, 128, 32, 32};
        frames.push_back(frame);

    } else if(explosion_type == 1){
        frame = {304, 128, 32, 32};
        frames.push_back(frame);

        frame = {336, 128, 32, 32};
        frames.push_back(frame);

        frame = {288, 160, 16, 16}; ///+100 point
        frames.push_back(frame);

    } else if(explosion_type == 2){
        frame = {256, 128, 16, 16};
        frames.push_back(frame);

        frame = {272, 128, 16, 16};
        frames.push_back(frame);

        frame = {288, 128, 16, 16};
        frames.push_back(frame);
    }
}

void Explosion::update(Uint32 deltaTime) {
    if (finished || frames.empty())
        return;

    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime -= frameTime;
        currentFrame++;
        if (currentFrame >= static_cast<int>(frames.size())) {
            finished = true;
        }
    }
}

void Explosion::render(SDL_Renderer* renderer) {
    if (finished || frames.empty())
        return;

    SDL_RenderCopy(renderer, texture, &frames[currentFrame], &position);
}

bool Explosion::isFinished() const {
    return finished;
}
