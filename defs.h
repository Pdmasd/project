//def.h
#ifndef DEFS_H
#define DEFS_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int FULL_SCREEN_WIDTH = 800;
const int FULL_SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int HIT_BOX = 36;

const float MOVE_SPEED = 2.5;
const float BULLET_SPEED = 3;
const int ENEMY_SHOOT_CHANGE = 5;     ///(%)

const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

inline const char * WINDOW_TITLE = "Battle City Remake";

#endif
