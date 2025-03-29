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
    Animation* anim;  // Con trỏ đến animation của explosion
    SDL_Rect rect;    // Vị trí và kích thước render của explosion
};

class Game {
private:
    Base* base;
    ScoreManager score;

    vector<Wall*> walls;
    vector<EnemyTank*> enemies;
    vector<AIController*> aiControllers;
    vector<Explosion*> explosionList;

    EnemySpawner* enemySpawner;
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
    bool pause;
    bool game_over = false;

    PlayerTank player;
    int spawnedEnemies = 0;
    int enemyNumber = 5;

    Game();

    void render();
    void update();
    ~Game();

    void generateWalls();
    void handleEvents();

    void spawnEnemies();
    void spawnEnemy(SDL_Texture* texture);

    void checkGameOver();
    void run();
    void resetGame();
};


#endif // GAME_H
