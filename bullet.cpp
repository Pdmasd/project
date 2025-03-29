//bullet.cpp
#include "bullet.h"

Bullet::Bullet (int startX, int startY, int dirX, int dirY) {
    x = startX;
    y = startY;
    dx = dirX;
    dy = dirY;
    active = true;
    rect = {x, y, 10, 10};
}

void Bullet::move() {
    if (!active) return;

    x += dx * BULLET_SPEED;
    y += dy * BULLET_SPEED;
    rect.x = x;
    rect.y = y;

    if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE ||
        y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (active) {
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Bullet::checkCollision(const Wall& wall) const {
    if (!wall.isActive() || wall.getType() == WallType::LEAF || wall.getType() == WallType::WATER) {
        return false;
    }
    return SDL_HasIntersection(&rect, &wall.rect);
}
