#include "EnemySpawner.h"
#include "AIController.h"
#include <cstdlib> // Cho rand()

EnemySpawner::EnemySpawner(SDL_Texture* enemyTexture,
                           std::vector<EnemyTank*>& enemies,
                           std::vector<AIController*>& aiControllers,
                           const std::vector<Wall*>* walls,
                           int playerX, int playerY,
                           int baseX, int baseY,
                           PlayerTank* player)
    : enemyTexture(enemyTexture), enemies(enemies), aiControllers(aiControllers),
      walls(walls), playerX(playerX), playerY(playerY), baseX(baseX), baseY(baseY), player(player)
{
    if (!enemyTexture) {
        std::cerr << "enemyTexture không hợp lệ!" << std::endl;
    }
}

void EnemySpawner::spawnEnemies(int maxEnemies, int spawnInterval) {
    static Uint32 lastSpawnTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    // Nếu số lượng enemy đã đạt giới hạn hoặc chưa đủ thời gian spawn thì thoát
    if (enemies.size() >= static_cast<size_t>(maxEnemies) ||
        currentTime - lastSpawnTime < static_cast<Uint32>(spawnInterval)) {
        return;
    }

    int spawnX = 0, spawnY = 0;
    if (findValidSpawnPosition(spawnX, spawnY)) {
        // Tạo enemy mới tại vị trí spawn hợp lệ
        EnemyTank* newEnemy = new EnemyTank(spawnX, spawnY, enemyTexture);
        newEnemy->active = true;
        enemies.push_back(newEnemy);

        aiControllers.push_back(new AIController(newEnemy, walls, &enemies, player));
        lastSpawnTime = currentTime;
    }
}

bool EnemySpawner::findValidSpawnPosition(int& outX, int& outY) {
    constexpr int maxAttempts = 100; // Số lần thử tối đa

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        int spawnX = (rand() % (SCREEN_WIDTH / TILE_SIZE - 2) + 1) * TILE_SIZE;
        int spawnY = TILE_SIZE;

        SDL_Rect spawnRect = { spawnX, spawnY, TILE_SIZE, TILE_SIZE };
        bool valid = true;

        // Kiểm tra va chạm với tường
        for (const auto& wall : *walls) {
            if (wall->active && SDL_HasIntersection(&spawnRect, &wall->rect)) {
                valid = false;
                break;
            }
        }

        for (const auto& enemy : enemies) {
            if (enemy->active && SDL_HasIntersection(&spawnRect, &enemy->rect)) {
                valid = false;
                break;
            }
        }

        // Nếu vị trí hợp lệ, trả về spawn position
        if (valid) {
            outX = spawnX;
            outY = spawnY;
            return true;
        }
    }

    return false;
}
