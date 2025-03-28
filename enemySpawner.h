#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include "player.h"
#include "Enemy.h"
#include "AIController.h"
#include "Wall.h"
#include "defs.h"

class EnemySpawner {
public:
    EnemySpawner(SDL_Texture* enemyTexture,
                 std::vector<EnemyTank*>& enemies,
                 std::vector<AIController*>& aiControllers,
                 const std::vector<Wall*>* walls,
                 int playerX, int playerY,
                 int baseX, int baseY,
                 PlayerTank* player);

    void spawnEnemies(int maxEnemies, int spawnInterval);
    vector<EnemyTank*>& GetEnemies() { return enemies; }

private:
    SDL_Texture* enemyTexture;
    std::vector<EnemyTank*>& enemies;
    std::vector<AIController*>& aiControllers;
    const std::vector<Wall*>* walls;

    int playerX;
    int playerY;
    int baseX;
    int baseY;

    PlayerTank* player;
    bool findValidSpawnPosition(int& outX, int& outY);
};

#endif // ENEMYSPAWNER_H
