//player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "wall.h"
#include "bullet.h"
#include "animation.h"
#include "wall.h"

#include<algorithm>

class EnemyTank;

class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet> bullets;

    bool was_moved = false;

    PlayerTank(int startX, int startY, SDL_Renderer* renderer);
    ~PlayerTank();

    SDL_Texture* texture;   // Sprite sheet của tank
    Animation animation;
    Animation* currentAnim; // Animation hiện tại
    Animation upAnim, downAnim, leftAnim, rightAnim;

    void move(int dx, int dy, const vector<Wall*>& walls, const vector<EnemyTank*>& enemies);

    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
};

#endif
