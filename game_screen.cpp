// game_screen.cpp
#include "game_screen.h"
#include "defs.h"

GameScreen::GameScreen(SDL_Renderer* renderer) : renderer(renderer) {}

void GameScreen::showStartScreen(bool& running) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};
    renderText("BATTLE CITY", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 3, white);
    renderText("Press ENTER to Start", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, white);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool waiting = true;
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                return;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                waiting = false;
            }
        }
    }
}

void GameScreen::showEndScreen(bool playerWon) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 0, 255};

    if (playerWon) {
        renderText("YOU WIN!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, green);
    } else {
        renderText("GAME OVER", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2, red);
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(3000); // Hiện màn hình trong 3 giây trước khi thoát
}

void GameScreen::renderText(const char* message, int x, int y, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 36);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    if (!surface) {
        SDL_Log("Failed to create surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}
