//animation.h
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include "defs.h"

class Animation {
private:
    SDL_Texture* texture;
    vector<SDL_Rect> frames;            /// Danh sách frame
    int currentFrame;
    Uint32 lastFrameTime;               /// Thời điểm cập nhật frame cuối cùng
    int frameDelay;                     /// Delay giữa các frame (ms)

    bool looping;
    bool finished;

public:
    Animation();

    Animation(SDL_Texture* texture, int frameWidth, int frameHeight, int frameDelay);
    ~Animation();

    void addFrame(int x, int y);
    void update();                                      /// Cập nhật frame theo thời gian
    void render(SDL_Renderer* renderer, int x, int y);  /// Vẽ frame hiện tại
    void reset();                                       /// Reset về frame đầu tiên

    bool isFinished() const;

    void setLooping(bool looping);
};

#endif
