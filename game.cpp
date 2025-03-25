//game.cpp
#include "game.h"
#include "defs.h"

using namespace std;

SDL_Texture* enemyTexture = nullptr;

Game::Game(): player(100, 100, nullptr){
    running = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
    }
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
    }

    // Tải texture chung cho enemy
    enemyTexture = IMG_LoadTexture(renderer, "image/sheet.png");
    if (!enemyTexture) {
        cerr << "Lỗi: Không thể load enemy texture: " << IMG_GetError() << endl;
        running = false;
    }

    generateWalls();
    player = PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer);
    spawnEnemies();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
    SDL_RenderClear(renderer); // delete color

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
        for (int j = 1; j < MAP_WIDTH - 1; ++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    for (auto& wall : walls) {
        wall.render(renderer);
    }

    player.render(renderer);

    for(auto &enemy : enemies){
        enemy.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

Game::~Game() {
    SDL_DestroyTexture(enemyTexture); // Giải phóng texture chung
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//void Game::generateWalls() {
//    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
//        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
//            WallType type = (rand() % 4 == 0) ? WallType::STEEL : WallType::BRICK;
//            walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE, renderer, (type == WallType::BRICK ? "image/brick.png" : "image/steel.png"), type);
//        }
//    }
//
//    // Thêm một số khối LEAF và WATER làm chướng ngại vật
//    walls.emplace_back(5 * TILE_SIZE, 5 * TILE_SIZE, renderer, "image/leaf.png", WallType::LEAF);
//    walls.emplace_back(6 * TILE_SIZE, 6 * TILE_SIZE, renderer, "image/water.png", WallType::WATER);
//}

void Game::generateWalls() {
    std::ifstream mapFile("map.txt");
    if (!mapFile.is_open()) {
        SDL_Log("Không thể mở file map.txt");
        return;
    }

    vector<string> mapData;
    string line;
    while (getline(mapFile, line)) {
        mapData.push_back(line);
    }
    mapFile.close();

    // Duyệt qua mapData và tạo tường
    for (int y = 0; y < mapData.size(); ++y) {
        for (int x = 0; x < mapData[y].size(); ++x) {
            char tile = mapData[y][x];
            WallType type;
            const char* texturePath = nullptr;

            switch (tile) {
                case 'B':
                    type = WallType::BRICK;
                    texturePath = "image/brick.png";
                    break;
                case 'S':
                    type = WallType::STEEL;
                    texturePath = "image/steel.png";
                    break;
                case 'L':
                    type = WallType::LEAF;
                    texturePath = "image/leaf.png";
                    break;
                case 'W':
                    type = WallType::WATER;
                    texturePath = "image/water.png";
                    break;
                default:
                    continue; // Bỏ qua ô trống
            }
            walls.emplace_back(x * TILE_SIZE, y * TILE_SIZE, renderer, texturePath, type);
        }
    }

    SDL_Log("Bản đồ đã tải thành công!");
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: player.move(0, - MOVE_SPEED, walls, enemies); break;
                case SDLK_DOWN: player.move(0, MOVE_SPEED, walls, enemies); break;
                case SDLK_LEFT: player.move(- MOVE_SPEED, 0, walls, enemies); break;
                case SDLK_RIGHT: player.move(MOVE_SPEED, 0, walls, enemies); break;
                case SDLK_SPACE: player.shoot(); break;
            }
        }
    }
}

void Game::update() {
    player.updateBullets();

    ///Đạn player bắn tường
    for (auto& bullet : player.bullets) {
        for (auto& wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                if (wall.type == WallType::BRICK) {
                    wall.active = false;
                    bullet.active = false;
                } else if (wall.type == WallType::STEEL) {
                    bullet.active = false;
                }
                break;
            }
        }
    }
    ///Đạn bắn enemy
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
            }
        }
    }

    ///enemy hành động
    for (auto& enemy : enemies) {
        enemy.move(walls, player, enemies);
        enemy.updateBullets();
        if (rand() % 100 < 2) {
            enemy.shoot();
        }
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            for (auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    if (wall.type == WallType::BRICK) {
                        wall.active = false;
                        bullet.active = false;
                    } else if (wall.type == WallType::STEEL) {
                        bullet.active = false;
                    }
                    break;
                }
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](EnemyTank& e) { return !e.active; }), enemies.end());

    if (enemies.empty()) {
        running = false;
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            // Update
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                running = false;
                return;
            }
        }
    }
}

void Game::spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        int maxAttempts = 100;

        while (!validPosition && maxAttempts--) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;

            SDL_Rect newEnemyRect = {ex, ey, TILE_SIZE, TILE_SIZE};

            for (const auto& wall : walls) {
                SDL_Rect wallRect = {wall.x, wall.y, TILE_SIZE, TILE_SIZE};
                if (wall.active && SDL_HasIntersection(&newEnemyRect, &wallRect)) {
                    validPosition = false;
                    break;
                }
            }

            for (const auto& enemy : enemies) {
                SDL_Rect enemyRect = {enemy.x, enemy.y, TILE_SIZE, TILE_SIZE};
                if (SDL_HasIntersection(&newEnemyRect, &enemyRect)) {
                    validPosition = false;
                    break;
                }
            }
        }
        if (validPosition) {
            enemies.emplace_back(ex, ey, enemyTexture);
        } else {
            std::cerr << "Cảnh báo: Không thể sinh thêm enemy!\n";
        }
    }
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}
