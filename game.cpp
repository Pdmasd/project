//game.cpp
#include "game.h"
#include "defs.h"

using namespace std;

SDL_Texture* enemyTexture = nullptr;
EnemySpawner* enemySpawner = nullptr;

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

    /// Hiển thị màn hình chờ
//    GameScreen screen(renderer);
//    screen.showMainMenu(running);

    /// Tải texture chung cho enemy
    enemyTexture = IMG_LoadTexture(renderer, "image/sheet.png");
//    enemySpawner = EnemySpawner::GetInstance(enemyTexture);

//    enemySpawner->StartSpawning(1);

    if (!enemyTexture) {
        cerr << "Lỗi: Không thể load enemy texture: " << IMG_GetError() << endl;
        running = false;
    }

    generateWalls();
    base = new Base((MAP_WIDTH / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "image/base.png");
    player = PlayerTank(((MAP_WIDTH) / 2) * TILE_SIZE, (MAP_HEIGHT - 4) * TILE_SIZE, renderer);

    enemySpawner = new EnemySpawner(enemyTexture, enemies, aiControllers, &walls, player.x, player.y, base->x, base->y, &player);
    enemySpawner->spawnEnemies(10, 3000);
}

Game::~Game() {
    delete base;
    SDL_DestroyTexture(enemyTexture); // Giải phóng texture chung

    for (auto enemy : enemies) {
        delete enemy;
    }
    for (auto ai : aiControllers) {
        delete ai;
    }
    delete enemySpawner;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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
    for (size_t y = 0; y < mapData.size(); ++y) {
        for (size_t  x = 0; x < mapData[y].size(); ++x) {
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
            int wallX = x * TILE_SIZE;
            int wallY = y * TILE_SIZE;

            // Nếu là Brick thì tạo nhóm 16 Brick nhỏ
            if (type == WallType::BRICK) {
                vector<Wall*> brickGroup = Wall::createBrickGroup(wallX, wallY, renderer, texturePath);
                for (auto brick : brickGroup) {
                    walls.push_back(brick);
                }
            } else {
                walls.push_back(new Wall(wallX, wallY, renderer, texturePath, type));
            }
        }
    }

    //SDL_Log("Bản đồ đã tải thành công!");
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }

    // Lấy trạng thái phím hiện tại
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    int dx = 0, dy = 0;
    if (keyState[SDL_SCANCODE_UP]) {
        dy = -MOVE_SPEED;
    } else if (keyState[SDL_SCANCODE_DOWN]) {
        dy = MOVE_SPEED;
    } else if (keyState[SDL_SCANCODE_LEFT]) {
        dx = -MOVE_SPEED;
    } else if (keyState[SDL_SCANCODE_RIGHT]) {
        dx = MOVE_SPEED;
    }

    // Cập nhật vị trí nếu có di chuyển
    if (dx != 0 || dy != 0) {
        player.move(dx, dy, walls, enemies);
    }

    /// Xử lý bắn đạn (giữ SPACE để bắn liên tục)
    static int bulletCooldown = 0;
    if (keyState[SDL_SCANCODE_SPACE] && bulletCooldown == 0) {
        player.shoot();
        bulletCooldown = 25;
    }
    if (bulletCooldown > 0) bulletCooldown--;
}

void Game::update() {
    player.updateBullets();

    ///Đạn player bắn tường
    for (auto& bullet : player.bullets) {
        for (auto& wall : walls) {
            if (wall->active && SDL_HasIntersection(&bullet.rect, &wall->rect)) {
                if (wall->type == WallType::BRICK) {
                    // Phá hủy brick bị trúng
                    wall->hit();
                    bullet.active = false;

                    int groupBaseX = (wall->x / TILE_SIZE) * TILE_SIZE;
                    int groupBaseY = (wall->y / TILE_SIZE) * TILE_SIZE;
                    int smallSize = TILE_SIZE / 4;
                    // Xác định vị trí (hàng, cột) của brick bị trúng trong nhóm (lưới 4x4)
                    int hitRow = (wall->y - groupBaseY) / smallSize;
                    int hitCol = (wall->x - groupBaseX) / smallSize;

                    // Nếu đạn được bắn theo chiều dọc (dirX == 0 và dirY != 0)
                    // thì hiệu ứng splash sẽ phá hủy các brick lân cận theo chiều ngang (cùng hàng)
                    if (bullet.dx == 0 && bullet.dy != 0) {
                        for (auto &otherBrick : walls) {
                            if (!otherBrick->active)
                                continue;
                            // Kiểm tra brick này có thuộc nhóm (ô chứa 16 brick nhỏ) không
                            if (otherBrick->x >= groupBaseX && otherBrick->x < groupBaseX + TILE_SIZE &&
                                otherBrick->y >= groupBaseY && otherBrick->y < groupBaseY + TILE_SIZE) {
                                int row = (otherBrick->y - groupBaseY) / smallSize;
                                int col = (otherBrick->x - groupBaseX) / smallSize;
                                // Nếu cùng hàng với brick bị trúng và khoảng cách cột không quá 2 ô
                                if (row == hitRow && abs(col - hitCol) <= 2) {
                                    otherBrick->hit();
                                }
                            }
                        }
                    }
                    // Nếu đạn bắn theo chiều ngang, bạn có thể xử lý splash theo chiều dọc (nếu cần)
                    else if (bullet.dy == 0 && bullet.dx != 0) {
                        for (auto &otherBrick : walls) {
                            if (!otherBrick->active)
                                continue;
                            if (otherBrick->x >= groupBaseX && otherBrick->x < groupBaseX + TILE_SIZE &&
                                otherBrick->y >= groupBaseY && otherBrick->y < groupBaseY + TILE_SIZE) {
                                int row = (otherBrick->y - groupBaseY) / smallSize;
                                int col = (otherBrick->x - groupBaseX) / smallSize;
                                // Nếu cùng cột với brick bị trúng và khoảng cách hàng không quá 2 ô
                                if (col == hitCol && abs(row - hitRow) <= 2) {
                                    otherBrick->hit();
                                }
                            }
                        }
                    }
                } else if (wall->type == WallType::STEEL) {
                    bullet.active = false;
                }
                break;
            }
        }
    }
    ///Đạn bắn enemy
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (enemy->active && SDL_HasIntersection(&bullet.rect, &enemy->rect)) {
                enemy->active = false;
                bullet.active = false;
            }
        }
    }

    enemySpawner->spawnEnemies(10, 3000);

    /// Cập nhật AI cho mỗi enemy thông qua các AIController
    for (auto ai : aiControllers) {
        ai->Update();
    }


    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->bullets) {
            for (auto& wall : walls) {
                if (wall->active && SDL_HasIntersection(&bullet.rect, &wall->rect)) {
                    if (wall->type == WallType::BRICK) {
                        // Phá hủy brick bị trúng
                        wall->hit();
                        bullet.active = false;

                        int groupBaseX = (wall->x / TILE_SIZE) * TILE_SIZE;
                        int groupBaseY = (wall->y / TILE_SIZE) * TILE_SIZE;
                        int smallSize = TILE_SIZE / 4;
                        // Xác định vị trí (hàng, cột) của brick bị trúng trong nhóm (lưới 4x4)
                        int hitRow = (wall->y - groupBaseY) / smallSize;
                        int hitCol = (wall->x - groupBaseX) / smallSize;

                        // Nếu đạn được bắn theo chiều dọc (dirX == 0 và dirY != 0)
                        // thì hiệu ứng splash sẽ phá hủy các brick lân cận theo chiều ngang (cùng hàng)
                        if (bullet.dx == 0 && bullet.dy != 0) {
                            for (auto &otherBrick : walls) {
                                if (!otherBrick->active)
                                    continue;
                                // Kiểm tra brick này có thuộc nhóm (ô chứa 16 brick nhỏ) không
                                if (otherBrick->x >= groupBaseX && otherBrick->x < groupBaseX + TILE_SIZE &&
                                    otherBrick->y >= groupBaseY && otherBrick->y < groupBaseY + TILE_SIZE) {
                                    int row = (otherBrick->y - groupBaseY) / smallSize;
                                    int col = (otherBrick->x - groupBaseX) / smallSize;
                                    // Nếu cùng hàng với brick bị trúng và khoảng cách cột không quá 2 ô
                                    if (row == hitRow && abs(col - hitCol) <= 2) {
                                        otherBrick->hit();
                                    }
                                }
                            }
                        }
                        // Nếu đạn bắn theo chiều ngang, bạn có thể xử lý splash theo chiều dọc (nếu cần)
                        else if (bullet.dy == 0 && bullet.dx != 0) {
                            for (auto &otherBrick : walls) {
                                if (!otherBrick->active)
                                    continue;
                                if (otherBrick->x >= groupBaseX && otherBrick->x < groupBaseX + TILE_SIZE &&
                                    otherBrick->y >= groupBaseY && otherBrick->y < groupBaseY + TILE_SIZE) {
                                    int row = (otherBrick->y - groupBaseY) / smallSize;
                                    int col = (otherBrick->x - groupBaseX) / smallSize;
                                    // Nếu cùng cột với brick bị trúng và khoảng cách hàng không quá 2 ô
                                    if (col == hitCol && abs(row - hitRow) <= 2) {
                                        otherBrick->hit();
                                    }
                                }
                            }
                        }
                    } else if (wall->type == WallType::STEEL) {
                        bullet.active = false;
                    }
                    break;
                }
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                        [](EnemyTank* e) { return !e->active; }), enemies.end());

//    if (spawnedEnemies == enemyNumber && enemies.empty()) {
//        game_over = true; // WIN khi tất cả kẻ địch bị tiêu diệt
//    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->bullets) {
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                running = false;
                return;
            }
        }
    }

    ///Kiểm tra nhà chính bị hủy chưa
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->bullets) {
            if (bullet.active && SDL_HasIntersection(&bullet.rect, &base->rect)) {
                SDL_Log("Nhà chính bị phá hủy!");
                base->destroy();  // Đánh dấu nhà chính bị phá hủy
                game_over = true;
                return;
            }
        }
    }
    for (auto& bullet : player.bullets) {
        if (bullet.active && SDL_HasIntersection(&bullet.rect, &base->rect)) {
            SDL_Log("Nhà chính bị phá hủy!");
            base->destroy();  // Đánh dấu nhà chính bị phá hủy
            game_over = true;
            return;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
        for (int j = 1; j < MAP_WIDTH - 1; ++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    player.render(renderer);

    for(auto &enemy : enemies){
        enemy->render(renderer);
    }

    base->render(renderer);

    for (auto& wall : walls) {
        wall->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {

    while (running) {
        handleEvents();
        update();
        render();

        if (game_over) {
            running = false; // Dừng game khi game_over = true
        }

        SDL_Delay(16);
    }
}




