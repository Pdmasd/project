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
    int dirX, dirY;             ///Hướng di chuyển
    int initialX, initialY;     ///Điểm hồi sinh
    SDL_Rect rect;
    vector<Bullet> bullets;

    bool was_moved = false;

    PlayerTank(int startX, int startY, SDL_Renderer* renderer);
    ~PlayerTank();

    SDL_Texture* texture;
    Animation animation;
    Animation* currentAnim;
    Animation upAnim, downAnim, leftAnim, rightAnim;

    /// Mạng
    bool alive;
    int lives;
    Uint32 deathTime;               /// Thời điểm tank chết (ms)
    Uint32 respawnDelay;     ///Thời gian chờ hồi sinh (ms)

    void die();
    void respawn();
    void updateStatus();        /// Cập nhật trạng thái

    void move(int dx, int dy, const vector<Wall*>& walls, const vector<EnemyTank*>& enemies);

    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
};

#endif
