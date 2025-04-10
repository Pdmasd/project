//enemy.cpp
#include "enemy.h"
#include "player.h"
#include "SoundManager.h"

EnemyTank::EnemyTank(int startX, int startY, SDL_Texture* sharedTexture, bool enhanced)
    : isEnhanced(enhanced)
{
    moveDelay = 15;
    shootDelay = 5;
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = MOVE_SPEED;
    active = true;

    texture = sharedTexture;

    /// Khởi tạo animation theo hướng
    upAnim = Animation(texture, 16, 16, 100);
    downAnim = Animation(texture, 16, 16, 100);
    leftAnim = Animation(texture, 16, 16, 100);
    rightAnim = Animation(texture, 16, 16, 100);

    /// Thêm frame cho từng hướng
    int yOffset;

    if(enhanced){
        yOffset = 80;
    } else{
        yOffset = 0;
    }

    upAnim.addFrame(128, yOffset);
    upAnim.addFrame(144, yOffset);

    downAnim.addFrame(192, yOffset);
    downAnim.addFrame(208, yOffset);

    leftAnim.addFrame(160, yOffset);
    leftAnim.addFrame(176, yOffset);

    rightAnim.addFrame(224, yOffset);
    rightAnim.addFrame(240, yOffset);

    currentAnim = &downAnim; /// Mặc định quay xuống
}

EnemyTank::~EnemyTank() {
    texture = nullptr;
}

void EnemyTank::shoot() {
    bool canShoot = true;
    if (--shootDelay > 0) return;
    shootDelay = 5;

    for (auto &bullet : bullets) {
        if (bullet.active) {
            canShoot = false;
            break;
        }
    }

    if (canShoot) {
        SoundManager::loadSound("shoot_bullet", "sound/shoot_bullet.wav");
        SoundManager::playSound("shoot_bullet", 0);

        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                                   this->dirX, this->dirY));
    }
}

void EnemyTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void EnemyTank::render(SDL_Renderer* renderer) {
    if (currentAnim) {
        currentAnim->render(renderer, x, y);  /// Render bằng animation
    }
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}
