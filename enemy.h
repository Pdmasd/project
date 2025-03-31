//enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "defs.h"
#include "wall.h"
#include "bullet.h"
#include "animation.h"
#include "wall.h"

#include<algorithm>

class PlayerTank;

class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;

    bool active;
    vector<Bullet> bullets;

    EnemyTank(int startX, int startY, SDL_Texture* sharedTexture);
    ~EnemyTank();

    SDL_Texture* texture;   // Sprite sheet của enemy
    Animation* currentAnim; // Animation hiện tại
    Animation upAnim, downAnim, leftAnim, rightAnim;

    EnemyTank(EnemyTank&& other) noexcept;

    EnemyTank& operator=(EnemyTank&& other) noexcept;

    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
};

#endif
