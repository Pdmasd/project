// base.h
#ifndef BASE_H
#define BASE_H

#include "defs.h"

class Base {
public:
    int x, y;                 // Vị trí nhà chính
    SDL_Rect rect;            // Kích thước và vị trí
    SDL_Texture* texture;     // Hình ảnh nhà chính
    bool destroyed;          // Trạng thái nhà chính (bị phá hủy hay chưa)

    Base(int startX, int startY, SDL_Renderer* renderer, const char* texturePath);
    ~Base();

    void render(SDL_Renderer* renderer);
    bool isDestroyed() const;
    void destroy();
};

#endif // BASE_H
