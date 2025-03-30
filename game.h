//game.h
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "defs.h"
#include "wall.h"
#include "player.h"
#include "enemy.h"
#include "base.h"
#include "game_screen.h"
#include "enemySpawner.h"
#include "AIController.h"
#include "scoreManager.h"
#include "explosion.h"

#include <cstdlib>
#include <fstream>
struct ExplosionAnimation {
    Animation* anim;
    SDL_Rect rect;
};

class Game {
private:
    Base* base;
    vector<AIController*> aiControllers;
    vector<Explosion*> explosionList;

    EnemySpawner* enemySpawner;
    int spawnedEnemies = 0;
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* enemyTexture = nullptr;

    PlayerTank player;
    PlayerTank* player2;

    vector<Wall*> walls;
    vector<EnemyTank*> enemies;

    ScoreManager score;

    bool continueGame;

    bool running;
    bool pause;
    bool game_over;
    int currentLevel;     /// Cấp độ hiện tại
    int enemyCount1;
    int enemyCount2;
    int enemyCount3;

    Game();
    ~Game();

    void render();
    void update();

    void generateWalls(const std::string& mapFile);
    void handleEvents();

    void spawnEnemies();
    void spawnEnemy(SDL_Texture* texture);

    void checkGameOver();
    void run();
    void resetGame();

    bool playAgain;
};


#endif // GAME_H
