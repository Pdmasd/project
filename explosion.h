#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "defs.h"

class Explosion {
public:
    Explosion(SDL_Texture* texture, int x, int y, int explosion_type);
    ~Explosion();

    void update(Uint32 deltaTime);

    void render(SDL_Renderer* renderer);

    bool isFinished() const;

    int explosion_type;         /// 0: player   1: enemy    2: wall
private:
    SDL_Texture* texture;
    SDL_Rect position;              /// Vị trí và kích thước hiển thị của explosion
    std::vector<SDL_Rect> frames;
    int currentFrame;
    Uint32 frameTime;               /// Thời gian hiển thị mỗi frame (ms)
    Uint32 elapsedTime;             /// Thời gian đã trôi qua kể từ frame hiện tại
    bool finished;

    void initFrames(int explosion_type);
};

#endif // EXPLOSION_H
