// player.cpp
#include "player.h"

void Player::init(Graphic& graphic, const char* filename, int startX, int startY, int moveSpeed) {
    x = startX;
    y = startY;
    speed = moveSpeed;
    velocityX = 0;
    velocityY = 0;
    texture = graphic.loadTexture(filename);
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:    velocityY = -speed; break;
            case SDLK_DOWN:  velocityY = speed; break;
            case SDLK_LEFT:  velocityX = -speed; break;
            case SDLK_RIGHT: velocityX = speed; break;
        }
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:    if (velocityY < 0) velocityY = 0; break;
            case SDLK_DOWN:  if (velocityY > 0) velocityY = 0; break;
            case SDLK_LEFT:  if (velocityX < 0) velocityX = 0; break;
            case SDLK_RIGHT: if (velocityX > 0) velocityX = 0; break;
        }
    }
}

void Player::update() {
    x += velocityX;
    y += velocityY;
}

void Player::render(Graphic& graphic) {
    graphic.renderTexture(texture, x, y, 100, 100);
}

void Player::cleanUp() {
    SDL_DestroyTexture(texture);
}
