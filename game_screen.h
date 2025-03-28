// game_screen.h
#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "defs.h"

class GameScreen {
public:
    GameScreen(SDL_Renderer* renderer);
     ~GameScreen();

    void showMainMenu(bool& running);
    void showGameOver(SDL_Renderer* renderer, const std::string& message);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    void renderText(const std::string& text, int x, int y, SDL_Color color);
};

#endif // GAME_SCREEN_H

