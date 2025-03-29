// game_screen.h
#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "defs.h"
#include "SoundManager.h"
#include "player.h"
#include "ScoreManager.h"

class GameScreen {
public:
    GameScreen(SDL_Renderer* renderer);
     ~GameScreen();

    void showMainMenu(SDL_Renderer* renderer, bool& running);
    void showGameOver(SDL_Renderer* renderer, int score, bool& backToMenu);
    void showStageIntro(SDL_Renderer* renderer, int stageNumber);
    void showPauseGame(SDL_Renderer* renderer);
    void figures(int lives, int score);

    PlayerTank player(int startX, int startY, SDL_Renderer* renderer);
private:
    SDL_Renderer* renderer;
    SDL_Texture* background;
    TTF_Font* font;

    void renderText(const std::string& text, int x, int y, SDL_Color color);
};

#endif // GAME_SCREEN_H

