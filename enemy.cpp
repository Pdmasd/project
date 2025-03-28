//enemy.cpp
#include "enemy.h"
#include "player.h"

EnemyTank::EnemyTank(int startX, int startY, SDL_Texture* sharedTexture) {
    moveDelay = 15; // Delay for movement
    shootDelay = 5; // Delay for shooting
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = 1;
    active = true;

    texture = sharedTexture; // Sử dụng texture chung

    // Khởi tạo animation theo hướng
    upAnim = Animation(texture, 16, 16, 100);
    downAnim = Animation(texture, 16, 16, 100);
    leftAnim = Animation(texture, 16, 16, 100);
    rightAnim = Animation(texture, 16, 16, 100);

    // Thêm frame cho từng hướng
    upAnim.addFrame(128, 0);
    upAnim.addFrame(144, 0);

    downAnim.addFrame(192, 0);
    downAnim.addFrame(208, 0);

    leftAnim.addFrame(160, 0);
    leftAnim.addFrame(176, 0);

    rightAnim.addFrame(224, 0);
    rightAnim.addFrame(240, 0);

    currentAnim = &downAnim; // Mặc định quay xuống
}

EnemyTank::~EnemyTank() {
    texture = nullptr;
}

// Move constructor
EnemyTank::EnemyTank(EnemyTank&& other) noexcept
    : x(other.x), y(other.y), dirX(other.dirX), dirY(other.dirY), rect(other.rect),
      active(other.active), texture(other.texture),
      upAnim(std::move(other.upAnim)), downAnim(std::move(other.downAnim)),
      leftAnim(std::move(other.leftAnim)), rightAnim(std::move(other.rightAnim)),
      bullets(std::move(other.bullets)) {
    currentAnim = other.currentAnim;
    other.texture = nullptr; // Đảm bảo texture không bị giải phóng
}

// Move assignment
EnemyTank& EnemyTank::operator=(EnemyTank&& other) noexcept {
    if (this != &other) {
        x = other.x;
        y = other.y;
        dirX = other.dirX;
        dirY = other.dirY;
        rect = other.rect;
        active = other.active;

        // Di chuyển animation và texture
        texture = other.texture;
        upAnim = std::move(other.upAnim);
        downAnim = std::move(other.downAnim);
        leftAnim = std::move(other.leftAnim);
        rightAnim = std::move(other.rightAnim);
        bullets = std::move(other.bullets);
        currentAnim = other.currentAnim;

        other.texture = nullptr;
    }
    return *this;
}

void EnemyTank::shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                                this->dirX, this->dirY));
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
        currentAnim->render(renderer, x, y);  // Render bằng animation
    }
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

