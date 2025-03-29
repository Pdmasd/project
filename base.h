// base.h
#ifndef BASE_H
#define BASE_H

#include "defs.h"

class Base {
public:
    int x, y;
    SDL_Rect rect;
    SDL_Texture* texture;
    bool destroyed;

    Base(int startX, int startY, SDL_Renderer* renderer, const char* texturePath);
    ~Base();

    void render(SDL_Renderer* renderer);
    bool isDestroyed() const;
    void destroy();
};

#endif // BASE_H
