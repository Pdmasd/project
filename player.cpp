//player.cpp
#include "player.h"
#include "enemy.h"

PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = -1; // Default direction up

    // Load sprite sheet tank.png
    SDL_Surface* surface = IMG_Load("image/sheet.png");

    if (!surface) {
    cout << "Lỗi tải hình ảnh: " << IMG_GetError() << endl;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Khởi tạo animation cho từng hướng
    upAnim = Animation(texture, 16, 16, 100);
    downAnim = Animation(texture, 16, 16, 100);
    leftAnim = Animation(texture, 16, 16, 100);
    rightAnim = Animation(texture, 16, 16, 100);

    // Thêm frame theo hướng di chuyển
    upAnim.addFrame(0, 0);
    upAnim.addFrame(16, 0);

    leftAnim.addFrame(32, 0);
    leftAnim.addFrame(48, 0);

    downAnim.addFrame(64, 0);
    downAnim.addFrame(80, 0);

    rightAnim.addFrame(96, 0);
    rightAnim.addFrame(112, 0);

    // Mặc định hướng lên
    currentAnim = &upAnim;
}

PlayerTank::~PlayerTank() {
    // Không cần giải phóng animation vì SDL_Texture* sẽ do lớp Animation xử lý.
}

void PlayerTank::shoot() {
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,
                                this->dirX, this->dirY));
}

void PlayerTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet &b) { return !b.active; }), bullets.end());
}

void PlayerTank::render(SDL_Renderer* renderer) {
    currentAnim->render(renderer, x, y);

    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

void PlayerTank::move(int dx, int dy, const vector<Wall>& walls, const vector<EnemyTank>& enemies) {
    this->dirX = dx;
    this->dirY = dy;

    // Chọn animation theo hướng di chuyển
    if (dy < 0) currentAnim = &upAnim;    // Lên
    else if (dy > 0) currentAnim = &downAnim; // Xuống
    else if (dx < 0) currentAnim = &leftAnim; // Trái
    else if (dx > 0) currentAnim = &rightAnim; // Phải

    int newX = x + dx;
    int newY = y + dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect) && wall.type != WallType::LEAF ) {
            return;
        }
    }

    // Kiểm tra va chạm với các enemy
    for (const auto& enemy : enemies) {
        if (enemy.active && SDL_HasIntersection(&newRect, &enemy.rect)) {
            return; // Ngừng di chuyển nếu va chạm với enemy
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }

//    if (newX >= TILE_SIZE && newX <= PLAY_SPACE_WIDTH - TILE_SIZE * 2 &&
//        newY >= TILE_SIZE && newY <= PLAY_SPACE_HEIGHT - TILE_SIZE * 2) {
//        x = newX;
//        y = newY;
//        rect.x = x;
//        rect.y = y;
//    }

    // Cập nhật frame animation

    if (currentAnim) {
        currentAnim->update();
    }
}
