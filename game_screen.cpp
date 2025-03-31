// game_screen.cpp
#include "game_screen.h"

GameScreen::GameScreen(SDL_Renderer* renderer) : renderer(renderer) {
    if (TTF_Init() == -1) {
        std::cerr << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 48);
    if (!font) {
        std::cerr << "Không thể tải font: " << TTF_GetError() << std::endl;
    }
}

GameScreen::~GameScreen() {
    if (font) TTF_CloseFont(font);
    TTF_Quit();
}


void GameScreen::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void GameScreen::showMainMenu(SDL_Renderer* renderer, bool &isRunning, bool &isTwoPlayerMode) {

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "image/menu_background.png");
    SDL_Texture* pointerTexture = IMG_LoadTexture(renderer, "image/pointer.png");

    SoundManager::loadSound("menu", "sound/menu.mp3");
    SoundManager::playSound("menu", -1);

    if (!backgroundTexture || !pointerTexture) {
        SDL_Log("Failed to load menu textures: %s", SDL_GetError());
        return;
    }

    int selectedItem = 1;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    selectedItem--;
                    if (selectedItem < 1) selectedItem = 3;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    selectedItem++;
                    if (selectedItem > 3) selectedItem = 1;
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (selectedItem == 1) {
                        isTwoPlayerMode = false;
                        SoundManager::freeSound("menu");
                        return;
                    } else if (selectedItem == 2) {
                        isTwoPlayerMode = true;
                        SoundManager::freeSound("menu");
                        return;
                    } else if (selectedItem == 3) {
                        isRunning = false;
                    }
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        //SDL_Color gray = {128, 128, 128, 255};
        SDL_Color yellow = {255, 223, 0, 255};
        SDL_Color orange = {255, 165, 0, 255};
        font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 48);

        //renderText("Continue", 350, 400, (selectedItem == 0) ? (saveExists ? yellow : gray) : (saveExists ? orange : gray));
        renderText("1  Player", 300, 450, (selectedItem == 1) ? yellow : orange);
        renderText("2  Players", 300, 500, (selectedItem == 2) ? yellow : orange);
        renderText("Exit", 300, 550, (selectedItem == 3) ? yellow : orange);

        SDL_Rect pointerRect = {250, 0, 32, 32};

        if (selectedItem == 1)
            pointerRect.y = 460;
        else if (selectedItem == 2)
            pointerRect.y = 510;
        else if (selectedItem == 3)
            pointerRect.y = 560;

        SDL_RenderCopy(renderer, pointerTexture, nullptr, &pointerRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(pointerTexture);
    TTF_CloseFont(font);
    TTF_Quit();
}

void GameScreen::showStageIntro(SDL_Renderer* renderer, int stageNumber) {
    SoundManager::loadSound("stageIntroMusic", "sound/stage_start.wav");
    SoundManager::playSound("stageIntroMusic", 0);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    SDL_Color yellow = {255, 255, 0, 255};
    font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 48);

    string stageText = "Stage " + to_string(stageNumber);

    renderText(stageText, 300, 250, yellow);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);
}

void GameScreen::showPauseGame(SDL_Renderer* renderer){
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color yellow = {255, 223, 0, 255};
    font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 48);

    renderText("GAME  PAUSED", 160, 250, red);

    font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 36);
    renderText("Press  P  to  continue!", 130, 300, yellow);
    renderText("Press  O  to  game  over!", 130, 330, yellow);
    renderText("Press  M  to  back  main menu!", 130, 360, yellow);
}

void GameScreen::figures(int P1_lives, int P2_lives, int score, int remainingEnemies, bool isTwoPlayerMode) {
    SDL_Color orange = {255, 165, 0, 255};

    font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 24);

    string livesText = "P1  LIVES    " + to_string(P1_lives);
    renderText(livesText, SCREEN_WIDTH + 20, 400, orange);
    if(isTwoPlayerMode){
        string livesText = "P2  LIVES    " + to_string(P2_lives);
        renderText(livesText, SCREEN_WIDTH + 20, 450, orange);
    }
    string scoreText = "SCORE    " + to_string(score);
    renderText(scoreText, SCREEN_WIDTH + 20, 500, orange);

    string remainingEnemiesText = "REMAINING    " + to_string(remainingEnemies);
    renderText(remainingEnemiesText, SCREEN_WIDTH + 20, 200, orange);
}

void GameScreen::showGameOver(SDL_Renderer* renderer, int score, bool& backToMenu) {
    SDL_Texture* gameOverTexture = IMG_LoadTexture(renderer, "image/gameover.png");
    if (!gameOverTexture) {
        SDL_Log("Failed to load gameover texture: %s", IMG_GetError());
        return;
    }

    SoundManager::loadSound("gameover", "sound/gameover.mp3");
    SoundManager::playSound("gameover", 0);

    /// Thiết lập vị trí bắt đầu
    SDL_Rect destRect = {0, FULL_SCREEN_HEIGHT, FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT};

    bool showScore = false;
    while (destRect.y > 0) {
        SDL_RenderClear(renderer);

        destRect.y -= 5;
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &destRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    showScore = true;

    if (showScore) {
        SDL_Color yellow = {255, 223, 0, 255};
        SDL_Color orange = {255, 165, 0, 255};
        font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 48);

        string scoreText = "SCORE   " + to_string(score);
        renderText(scoreText, 250, 450, yellow);

        font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 36);
        renderText("Press  Enter  to  back  mainmenu", 150, 500, orange);

        SDL_RenderPresent(renderer);
    }

    bool waiting = true;
    SDL_Event e;
    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                backToMenu = false;
                waiting = false;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {

                backToMenu = true;
                waiting = false;
            }
        }
        SDL_Delay(10);
    }
    SDL_DestroyTexture(gameOverTexture);
}
void GameScreen::showVictory(SDL_Renderer* renderer, int score, bool& backToMenu) {
    SDL_Texture* victoryTexture = IMG_LoadTexture(renderer, "image/victory.png");
    if (!victoryTexture) {
        SDL_Log("Failed to load victory texture: %s", IMG_GetError());
        return;
    }

    SoundManager::loadSound("victory", "sound/victory.mp3");
    SoundManager::playSound("victory", 0);

    /// Thiết lập vị trí bắt đầu
    SDL_Rect destRect = {0, FULL_SCREEN_HEIGHT, FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT};

    bool showScore = false;
    while (destRect.y > 0) {
        SDL_RenderClear(renderer);

        destRect.y -= 8;
        SDL_RenderCopy(renderer, victoryTexture, nullptr, &destRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    showScore = true;

    if (showScore) {
        SDL_Color yellow = {255, 223, 0, 255};
        SDL_Color orange = {255, 165, 0, 255};
        font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 48);

        string scoreText = "SCORE   " + to_string(score + 95000);
        renderText(scoreText, 250, 450, yellow);

        font = TTF_OpenFont("ttf/ARCADECLASSIC.ttf", 36);
        renderText("Press  Enter  to  back  mainmenu", 150, 500, orange);

        SDL_RenderPresent(renderer);
    }

    bool waiting = true;
    SDL_Event e;
    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                backToMenu = false;
                waiting = false;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {

                backToMenu = true;
                waiting = false;
            }
        }
        SDL_Delay(10);
    }
    SDL_DestroyTexture(victoryTexture);
}
