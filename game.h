//game.h
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "defs.h"
#include "wall.h"
#include "player.h"
#include "enemy.h"

#include <cstdlib>
#include <fstream>
class Game {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 5;
    vector<EnemyTank> enemies;

    Game();

    void render();
    void update();
    ~Game();

    void generateWalls();
    void handleEvents();
    void spawnEnemies();

    void run();
};

#endif // GAME_H
