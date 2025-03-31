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
                 vector<EnemyTank*>& enemies,
                 vector<AIController*>& aiControllers,
                 const vector<Wall*>* walls,
                 int playerX, int playerY,
                 int baseX, int baseY,
                 PlayerTank* player,
                 PlayerTank* player2);

    void spawnEnemies(int maxEnemies, int spawnInterval);
    vector<EnemyTank*>& GetEnemies() { return enemies; }

    int getSpawnedCount() const { return enemySpawned; }

private:
    int enemySpawned = 0;

    SDL_Texture* enemyTexture;
    vector<EnemyTank*>& enemies;
    vector<AIController*>& aiControllers;
    const vector<Wall*>* walls;

    int playerX;
    int playerY;
    int player2X;
    int player2Y;

    //int enemyCount = 0;
    PlayerTank* player;
    PlayerTank* player2;
    bool findValidSpawnPosition(int& outX, int& outY);
};

#endif // ENEMYSPAWNER_H
