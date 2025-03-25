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

void EnemyTank::move(const vector<Wall>& walls, const PlayerTank& player, const std::vector<EnemyTank>& enemies) {
    if (--moveDelay > 0) return;
    moveDelay = 15;

    int r = rand() % 4;
    if (r == 0) { // Up
        this->dirX = 0;
        this->dirY = - MOVE_SPEED;
    }
    else if (r == 1) { // Down
        this->dirX = 0;
        this->dirY = MOVE_SPEED;
    }
    else if (r == 2) { // Left
        this->dirY = 0;
        this->dirX = - MOVE_SPEED;
    }
    else if (r == 3) { // Right
        this->dirY = 0;
        this->dirX = MOVE_SPEED;
    }

    // Chọn animation theo hướng di chuyển
    if (dirY < 0) currentAnim = &upAnim;    // Lên
    else if (dirY > 0) currentAnim = &downAnim; // Xuống
    else if (dirX < 0) currentAnim = &leftAnim; // Trái
    else if (dirX > 0) currentAnim = &rightAnim; // Phải

    int newX = x + this->dirX;
    int newY = y + this->dirY;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return;
        }
    }

    // Kiểm tra va chạm với Player
    if (SDL_HasIntersection(&newRect, &player.rect)) {
        return; // Không di chuyển nếu va chạm Player
    }

    // Kiểm tra va chạm với các enemy khác
    for (const auto& enemy : enemies) {
        if (&enemy != this && enemy.active && SDL_HasIntersection(&newRect, &enemy.rect)) {
            return; // Không di chuyển nếu va chạm với enemy khác
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
//    if (newX >= TILE_SIZE && newX <= PLAY_SPACE_WIDTH - TILE_SIZE * 2&&
//        newY >= TILE_SIZE && newY <= PLAY_SPACE_HEIGHT - TILE_SIZE * 2) {
//        x = newX;
//        y = newY;
//        rect.x = x;
//        rect.y = y;
//    }

    if (currentAnim) currentAnim->update();
}
