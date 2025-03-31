#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include "player.h"
#include "Enemy.h"
#include "Wall.h"
#include "defs.h"

class AIController {
public:
    /// Tham số enemyList và player có giá trị mặc định nullptr nếu không có
    AIController(EnemyTank* enemy,
                 const vector<Wall*>* walls,
                 const vector<EnemyTank*>* enemyList = nullptr,
                 PlayerTank* player = nullptr,
                 PlayerTank* player2 = nullptr);

    void Update();
    bool CanMove(int x, int y);
    void MovePatrol();

private:
    EnemyTank* controlledEnemy;
    const std::vector<Wall*>* walls;
    const std::vector<EnemyTank*>* enemyList;
    PlayerTank* player;
    PlayerTank* player2;

    Uint32 lastDirectionChange;
    Uint32 lastSideChange;
    const Uint32 directionCooldown = 1000; // Cooldown thay đổi hướng (1 giây)
};

#endif // AI_CONTROLLER_H
