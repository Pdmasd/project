// game_screen.h
#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SDL.h>
#include <SDL_ttf.h>

class GameScreen {
public:
    GameScreen(SDL_Renderer* renderer);
    void showStartScreen(bool& running);
    void showEndScreen(bool playerWon);

private:
    SDL_Renderer* renderer;
    void renderText(const char* message, int x, int y, SDL_Color color);
};

#endif // GAME_SCREEN_H

