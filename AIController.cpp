#include "AIController.h"
#include <cstdlib> // Cho rand()

AIController::AIController(EnemyTank* enemy,
                           const std::vector<Wall*>* walls,
                           const std::vector<EnemyTank*>* enemyList,
                           PlayerTank* player)
    : controlledEnemy(enemy), walls(walls), enemyList(enemyList), player(player)
{
    lastDirectionChange = SDL_GetTicks();
    lastSideChange = SDL_GetTicks();

    // Chọn hướng ban đầu ngẫu nhiên
    int r = rand() % 4;
    switch (r) {
        case 0: controlledEnemy->dirX = 0;            controlledEnemy->dirY = -MOVE_SPEED; break; // Up
        case 1: controlledEnemy->dirX = 0;            controlledEnemy->dirY =  MOVE_SPEED; break; // Down
        case 2: controlledEnemy->dirX = -MOVE_SPEED;   controlledEnemy->dirY = 0;           break; // Left
        case 3: controlledEnemy->dirX =  MOVE_SPEED;   controlledEnemy->dirY = 0;           break; // Right
    }
}

void AIController::Update() {
    if (!controlledEnemy || !controlledEnemy->active)
        return;

    Uint32 currentTime = SDL_GetTicks();
    Uint32 sideChangeCoolDown = 4000; // Ví dụ: 4 giây

    // Thử thay đổi hướng bên (trái/phải) với xác suất nhất định
    if (currentTime - lastSideChange >= sideChangeCoolDown) {
        int chance = rand() % 100;

        // Xác định hướng trái/phải dựa theo hướng hiện tại
        int leftX = 0, leftY = 0, rightX = 0, rightY = 0;
        if (controlledEnemy->dirY < 0) { // đang đi lên
            leftX = -MOVE_SPEED; rightX = MOVE_SPEED;
        } else if (controlledEnemy->dirY > 0) { // đang đi xuống
            leftX = MOVE_SPEED; rightX = -MOVE_SPEED;
        } else if (controlledEnemy->dirX < 0) { // đang đi trái
            leftY = MOVE_SPEED; rightY = -MOVE_SPEED;
        } else if (controlledEnemy->dirX > 0) { // đang đi phải
            leftY = -MOVE_SPEED; rightY = MOVE_SPEED;
        }

        // Với 20% khả năng đổi hướng sang trái
        if (chance < 20 && CanMove(controlledEnemy->x + leftX, controlledEnemy->y + leftY)) {
            controlledEnemy->dirX = leftX;
            controlledEnemy->dirY = leftY;
            lastSideChange = currentTime;
        }
        // Với 20% khả năng đổi hướng sang phải
        else if (chance >= 20 && chance < 40 && CanMove(controlledEnemy->x + rightX, controlledEnemy->y + rightY)) {
            controlledEnemy->dirX = rightX;
            controlledEnemy->dirY = rightY;
            lastSideChange = currentTime;
        }
    }

    // Di chuyển theo chế độ patrol (tuần tra)
    MovePatrol();

    // Xác suất bắn ngẫu nhiên (5%)
    if (rand() % 100 < 5) {
        controlledEnemy->shoot();
    }
    controlledEnemy->updateBullets();
}

void AIController::MovePatrol() {
    /// Lấy hướng hiện tại
    int dx = controlledEnemy->dirX;
    int dy = controlledEnemy->dirY;

    /// Tính vị trí mới
    int newX = controlledEnemy->x + dx;
    int newY = controlledEnemy->y + dy;
    SDL_Rect newRect = { newX, newY, HIT_BOX, HIT_BOX};

    /// Kiểm tra xem có thể di chuyển theo hướng hiện tại không
    if (!CanMove(newX, newY)) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastDirectionChange < directionCooldown)
            return;

        int origDx = dx, origDy = dy;
        int r = rand() % 4;
        switch (r) {
            case 0: dx = 0;          dy = -MOVE_SPEED; break; // Up
            case 1: dx = 0;          dy =  MOVE_SPEED; break; // Down
            case 2: dx = -MOVE_SPEED; dy = 0;           break; // Left
            case 3: dx =  MOVE_SPEED; dy = 0;           break; // Right
        }
        if (dx != origDx || dy != origDy)
            lastDirectionChange = currentTime;

        newX = controlledEnemy->x + dx;
        newY = controlledEnemy->y + dy;
        newRect = { newX, newY, HIT_BOX, HIT_BOX};

        if (!CanMove(newX, newY))
            return;

        /// Cập nhật hướng mới
        controlledEnemy->dirX = dx;
        controlledEnemy->dirY = dy;
    }

    /// Cập nhật vị trí mới cho xe tăng
    controlledEnemy->x = newX;
    controlledEnemy->y = newY;
    controlledEnemy->rect.x = newX;
    controlledEnemy->rect.y = newY;

    /// Cập nhật animation dựa theo hướng
    if (dy < 0)
        controlledEnemy->currentAnim = &controlledEnemy->upAnim;
    else if (dy > 0)
        controlledEnemy->currentAnim = &controlledEnemy->downAnim;
    else if (dx < 0)
        controlledEnemy->currentAnim = &controlledEnemy->leftAnim;
    else if (dx > 0)
        controlledEnemy->currentAnim = &controlledEnemy->rightAnim;

    if (controlledEnemy->currentAnim)
        controlledEnemy->currentAnim->update();
}

bool AIController::CanMove(int x, int y) {
    SDL_Rect newRect = { x, y, HIT_BOX, HIT_BOX};

    /// Kiểm tra va chạm với tường
    for (const auto& wall : *walls) {
        if (wall->active && SDL_HasIntersection(&newRect, &wall->rect))
            return false;
    }
    /// Kiểm tra giới hạn màn hình
    if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE * 2 ||
        y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE * 2)
        return false;

    /// Kiểm tra va chạm với các xe tăng địch khác
    if (enemyList) {
        for (const auto& enemy : *enemyList) {
            if (enemy != controlledEnemy && enemy->active &&
                SDL_HasIntersection(&newRect, &enemy->rect))
                return false;
        }
    }
    /// Kiểm tra va chạm với người chơi
    if (player) {
        if (SDL_HasIntersection(&newRect, &player->rect))
            return false;
    }
    return true;
}
