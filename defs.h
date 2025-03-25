//def.h
#ifndef DEFS_H
#define DEFS_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include<iostream>
#include<vector>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MOVE_SPEED = 10;

const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

inline const char * WINDOW_TITLE = "Game v0.0";

#endif
