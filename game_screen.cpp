// game_screen.cpp

#include "game_screen.h"

GameScreen::GameScreen(SDL_Renderer* renderer) : renderer(renderer) {
    if (TTF_Init() == -1) {
        std::cerr << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
    }
    font = TTF_OpenFont("image/arial.ttf", 48);
    if (!font) {
        std::cerr << "Không thể tải font: " << TTF_GetError() << std::endl;
    }
}

GameScreen::~GameScreen() {
    if (font) TTF_CloseFont(font);
    TTF_Quit();
}

// Vẽ văn bản lên màn hình
void GameScreen::renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Hiển thị màn hình chờ
void GameScreen::showMainMenu(bool& running) {
    bool inMenu = true;
    SDL_Event event;

    while (inMenu && running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inMenu = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                inMenu = false;  // Bắt đầu trò chơi khi nhấn Enter
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderText("TANK BATTLE", 220, 200, {255, 255, 255, 255});
        renderText("Press ENTER to Start", 150, 350, {255, 255, 255, 255});

        SDL_RenderPresent(renderer);
    }
}

// Hiển thị màn hình Game Over hoặc You Win
void GameScreen::showGameOver(SDL_Renderer* renderer, const std::string& message) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderText(message, 300, 300, {255, 0, 0, 255});
    renderText("Press ESC to Exit", 250, 400, {255, 255, 255, 255});

    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool waiting = true;
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                waiting = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                waiting = false;
            }
        }
    }
}
