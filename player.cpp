//player.cpp
#include "player.h"
#include "enemy.h"
#include "SoundManager.h"

PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer) :
        upAnim(nullptr, 16, 16, 100), downAnim(nullptr, 16, 16, 100),
        leftAnim(nullptr, 16, 16, 100), rightAnim(nullptr, 16, 16, 100),
        respawnDelay(2000)  {
    x = startX;
    y = startY;
    initialX = startX;
    initialY = startY;

    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = -1; /// mặc định hướng lên

    alive = true;
    lives = 3;
    deathTime = 0;

    SDL_Surface* surface = IMG_Load("image/sheet.png");

    if (!surface) {
    cout << "Lỗi tải hình ảnh: " << IMG_GetError() << endl;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    /// Khởi tạo animation cho từng hướng
    upAnim = Animation(texture, 16, 16, 100);
    downAnim = Animation(texture, 16, 16, 100);
    leftAnim = Animation(texture, 16, 16, 100);
    rightAnim = Animation(texture, 16, 16, 100);

    /// Thêm frame theo hướng di chuyển
    upAnim.addFrame(0, 0);
    upAnim.addFrame(16, 0);

    leftAnim.addFrame(32, 0);
    leftAnim.addFrame(48, 0);

    downAnim.addFrame(64, 0);
    downAnim.addFrame(80, 0);

    rightAnim.addFrame(96, 0);
    rightAnim.addFrame(112, 0);
}

PlayerTank::~PlayerTank() {}

void PlayerTank::shoot() {
    if (!alive) return;

    bool canShoot = true;       /// Viên đạn mới chỉ được bắn khi viên đạn cũ bị mất
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

void PlayerTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void PlayerTank::updateStatus() {
    updateBullets();

    if (!alive) {
        if (SDL_GetTicks() - deathTime >= respawnDelay) {
            respawn();
        }
    }
}

void PlayerTank::die() {
    if (alive) {
        alive = false;
        deathTime = SDL_GetTicks();
    }
}

void PlayerTank::respawn() {
    if (lives > 0) {
        lives--;
        x = initialX;
        y = initialY;
        rect.x = x;
        rect.y = y;
        alive = true;
        bullets.clear();
    } else {
        bool game_over = true;
    }
}

void PlayerTank::render(SDL_Renderer* renderer) {
    if (!alive) return;

    if(!was_moved){
        currentAnim = &upAnim;
        was_moved = true;
    }
    currentAnim->render(renderer, x, y);

    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

void PlayerTank::move(int dx, int dy, const vector<Wall*>& walls, const vector<EnemyTank*>& enemies) {
    if (!alive) return;

    this->dirX = dx;
    this->dirY = dy;

    /// Chọn animation theo hướng di chuyển
    if (dy < 0) currentAnim = &upAnim;          /// Lên
    else if (dy > 0) currentAnim = &downAnim;   /// Xuống
    else if (dx < 0) currentAnim = &leftAnim;   /// Trái
    else if (dx > 0) currentAnim = &rightAnim;  /// Phải

    int newX = x + dx;
    int newY = y + dy;

    SDL_Rect newRect = { newX, newY, HIT_BOX, HIT_BOX};
    for (const auto& wall : walls) {
        if (wall->active && SDL_HasIntersection(&newRect, &wall->rect) && wall->type != WallType::LEAF ) {
            return;
        }
    }

    /// Kiểm tra va chạm với các enemy
    for (const auto& enemy : enemies) {
        if (enemy->active && SDL_HasIntersection(&newRect, &enemy->rect)) {
            return;
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }

    if (currentAnim) {
        currentAnim->update();
    }
}
