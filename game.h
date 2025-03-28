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

#include <cstdlib>
#include <fstream>
class Game {
private:
    Base* base;
    vector<Wall*> walls;
    vector<EnemyTank*> enemies;
    vector<AIController*> aiControllers;

    EnemySpawner* enemySpawner;

public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
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
};

#endif // GAME_H
