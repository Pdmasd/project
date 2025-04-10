//game.cpp
#include "game.h"

using namespace std;

SDL_Texture* enemyTexture = nullptr;
EnemySpawner* enemySpawner = nullptr;
bool isTwoPlayerMode = false;

///Làm trong suốt phần màu đen trong sheet
SDL_Texture* loadTransparentSprite(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Lỗi khi load sheet.png: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 1));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

Game::Game(): player(0, 0, nullptr){
    running = true;
    pause = false;

    currentLevel = 1;
    enemyCount1 = 10;
    enemyCount2 = 15;
    enemyCount3 = 25;

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
    ///Khởi tạo SDL_mixer
    SoundManager::init();

    /// Hiển thị màn hình chờ
    GameScreen screen(renderer);
    screen.showMainMenu(renderer, running, isTwoPlayerMode);
    if(running){
        screen.showStageIntro(renderer, currentLevel);
    }

    /// Tải texture chung cho enemy
    enemyTexture = loadTransparentSprite("image/sheet.png", renderer);

    if (!enemyTexture) {
        cerr << "Lỗi: Không thể load enemy texture: " << IMG_GetError() << endl;
        running = false;
    }

    generateWalls("map/stage1.txt");
    base = new Base(7 * TILE_SIZE, 13 * TILE_SIZE, renderer, "image/base.png");
    player = PlayerTank(4 * TILE_SIZE, 13 * TILE_SIZE, renderer);

    if (isTwoPlayerMode) {
        player2 = new PlayerTank(10 * TILE_SIZE, 13 * TILE_SIZE, renderer);
        player2->initPlayer2Animations(player2->texture);
    } else {
        player2 = nullptr;
    }

    enemySpawner = new EnemySpawner(enemyTexture, enemies, aiControllers, &walls, player.x, player.y, base->x, base->y, &player, nullptr);
}

Game::~Game() {
    delete base;
    SDL_DestroyTexture(enemyTexture);

    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    for (auto ai : aiControllers) {
        delete ai;
    }
    aiControllers.clear();

    delete &player;

    if (player2 != nullptr) {
        delete player2;
    }

    delete enemySpawner;

    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::generateWalls(const std::string& mapFile) {
    ifstream file(mapFile);
    if (!file.is_open()) {
        SDL_Log("Không thể mở file %s", mapFile.c_str());
        return;
    }

    vector<string> mapData;
    string line;
    while (std::getline(file, line)) {
        mapData.push_back(line);
    }
    file.close();

    /// Xóa các wall từ stage trước
    for (auto wall : walls) {
        delete wall;
    }
    walls.clear();

    /// Duyệt qua mapData và tạo tường
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
                    continue;
            }
            int wallX = x * TILE_SIZE;
            int wallY = y * TILE_SIZE;

            /// Nếu là Brick thì tạo nhóm 16 Brick nhỏ
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
}

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_p) {
                SoundManager::loadSound("pause", "sound/pause.wav");
                SoundManager::playSound("pause", 0);

                if(pause)
                    pause = false;
                else if(!pause)
                    pause = true;
                break;
            } else if(event.key.keysym.sym == SDLK_o){
                if(pause){
                    pause = false;
                    resetGame();
                    game_over = true;
                }
                break;
            }  else if(event.key.keysym.sym == SDLK_m){
                if(pause){
                    pause = false;
                    resetGame();
                    GameScreen screen(renderer);
                    screen.showMainMenu(renderer, running, isTwoPlayerMode);
                }
                break;
            }
        }
    }


    if(pause){
        GameScreen screen(renderer);
        screen.showPauseGame(renderer);

        SDL_RenderPresent(renderer);
        return;
    }

    /// Lấy trạng thái phím hiện tại
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

    /// Cập nhật vị trí nếu có di chuyển
    if (dx != 0 || dy != 0) {
        player.move(dx, dy, walls, enemies, player2);
    }

    /// Xử lý bắn đạn (giữ SPACE để bắn liên tục)
    static int bulletCooldown = 0;
    if (keyState[SDL_SCANCODE_SPACE] && bulletCooldown == 0) {
        player.shoot();
        bulletCooldown = 25;
    }
    if (bulletCooldown > 0) bulletCooldown--;

    /// Player 2
    if (isTwoPlayerMode && player2 != nullptr) {
        int dx2 = 0, dy2 = 0;
        if (keyState[SDL_SCANCODE_W]) {
            dy2 = -MOVE_SPEED;
        } else if (keyState[SDL_SCANCODE_S]) {
            dy2 = MOVE_SPEED;
        } else if (keyState[SDL_SCANCODE_A]) {
            dx2 = -MOVE_SPEED;
        } else if (keyState[SDL_SCANCODE_D]) {
            dx2 = MOVE_SPEED;
        }
        if (dx2 != 0 || dy2 != 0) {
            player2->move(dx2, dy2, walls, enemies, &player);
        }

        static int bulletCooldown2 = 0;
        if (keyState[SDL_SCANCODE_E] && bulletCooldown2 == 0) {
            player2->shoot();
            bulletCooldown2 = 25;
        }
        if (bulletCooldown2 > 0) bulletCooldown2--;
    }
}

void Game::update() {
    player.updateStatus();

    if (isTwoPlayerMode && player2 != nullptr) {
        player2->updateStatus();
    }

    Uint32 currentTime = SDL_GetTicks();
    static Uint32 lastTime = currentTime; /// Khởi tạo lastTime tĩnh
    Uint32 deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    SoundManager::loadSound("explosion", "sound/explosion.wav");

    if (currentLevel == 3) {
        enemySpawner->spawnEnemies(enemyCount3, 2500);
    } else if (currentLevel == 2) {
        enemySpawner->spawnEnemies(enemyCount2, 3000);
    } else {
        enemySpawner->spawnEnemies(enemyCount1, 4000);
    }

    /// Cập nhật AI cho mỗi enemy thông qua các AIController
    for (auto ai : aiControllers) {
        ai->Update();
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                        [](EnemyTank* e) { return !e->active; }), enemies.end());


    ///Đạn enemy bắn trúng player
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy->bullets) {
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                bullet.active = false;
                player.die();

                Explosion* exp = new Explosion(enemyTexture, player.x, player.y, 0);
                explosionList.push_back(exp);

                SoundManager::playSound("explosion", 0);

                if(!isTwoPlayerMode){
                    if (!player.alive && player.lives == 0) {
                        game_over = true;
                        return;
                    }
                } else {
                    if (!player.alive && !player2->alive && player.lives == 0 && player2->lives == 0) {
                        game_over = true;
                        return;
                    }
                }
            }
        }
    }

    ///Đạn player trúng đạn enemy
    for(auto& p_bullet : player.bullets){
        for (auto& enemy : enemies) {
            for (auto& e_bullet : enemy->bullets) {
                if (SDL_HasIntersection(&p_bullet.rect, &e_bullet.rect)){
                    p_bullet.active = false;
                    e_bullet.active = false;
                }
            }
        }
    }

    ///Đạn enemy bắn trúng Player 2
    if (isTwoPlayerMode && player2 != nullptr) {
        for (auto& enemy : enemies) {
            for (auto& bullet : enemy->bullets) {
                if (SDL_HasIntersection(&bullet.rect, &player2->rect)) {
                    bullet.active = false;
                    player2->die();

                    Explosion* exp = new Explosion(enemyTexture, player2->x, player2->y, 0);
                    explosionList.push_back(exp);

                    SoundManager::playSound("explosion", 0);

                    if (!player.alive && !player2->alive && player.lives == 0 && player2->lives == 0) {
                        game_over = true;
                        return;
                    }
                }
            }
        }
    }

    ///Đạn player2 trúng đạn enemy
    if (isTwoPlayerMode && player2 != nullptr){
        for(auto& p_bullet : player2->bullets){
            for (auto& enemy : enemies) {
                for (auto& e_bullet : enemy->bullets) {
                    if (SDL_HasIntersection(&p_bullet.rect, &e_bullet.rect)){
                        p_bullet.active = false;
                        e_bullet.active = false;
                    }
                }
            }
        }
    }

    ///Kiểm tra va chạm đạn với tường
    auto handleBulletWallCollision = [&](std::vector<Bullet>& bullets) {
        for (auto& bullet : bullets) {
            for (auto& wall : walls) {
                if (wall->active && SDL_HasIntersection(&bullet.rect, &wall->rect)) {
                    if (wall->type == WallType::BRICK) {
                        wall->hit();
                        bullet.active = false;

                        Explosion* exp = new Explosion(enemyTexture, wall->rect.x, wall->rect.y, 2);
                        explosionList.push_back(exp);

                        SoundManager::loadSound("explosion", "sound/explosion.wav");
                        SoundManager::playSound("explosion", 0);

                        int groupBaseX = (wall->x / TILE_SIZE) * TILE_SIZE;
                        int groupBaseY = (wall->y / TILE_SIZE) * TILE_SIZE;
                        int smallSize = TILE_SIZE / 4;

                        /// Xác định vị trí của brick bị trúng
                        int hitRow = (wall->y - groupBaseY) / smallSize;
                        int hitCol = (wall->x - groupBaseX) / smallSize;

                        /// Nếu đạn được bắn theo chiều dọc
                        if (bullet.dx == 0 && bullet.dy != 0) {
                            for (auto &otherBrick : walls) {
                                if (!otherBrick->active)
                                    continue;

                                if (otherBrick->x >= groupBaseX && otherBrick->x < groupBaseX + TILE_SIZE &&
                                    otherBrick->y >= groupBaseY && otherBrick->y < groupBaseY + TILE_SIZE) {
                                    int row = (otherBrick->y - groupBaseY) / smallSize;
                                    int col = (otherBrick->x - groupBaseX) / smallSize;

                                    if (row == hitRow && abs(col - hitCol) <= 2) {
                                        otherBrick->hit();
                                    }
                                }
                            }
                        }
                        /// Nếu đạn bắn theo chiều ngang
                        else if (bullet.dy == 0 && bullet.dx != 0) {
                            for (auto &otherBrick : walls) {
                                if (!otherBrick->active)
                                    continue;
                                if (otherBrick->x >= groupBaseX && otherBrick->x < groupBaseX + TILE_SIZE &&
                                    otherBrick->y >= groupBaseY && otherBrick->y < groupBaseY + TILE_SIZE) {
                                    int row = (otherBrick->y - groupBaseY) / smallSize;
                                    int col = (otherBrick->x - groupBaseX) / smallSize;

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
    };

    ///Kiểm tra va chạm đạn với enemy
    auto handleBulletEnemyCollision = [&](std::vector<Bullet>& bullets, int scoreValue) {
        for (auto& bullet : bullets) {
            for (auto& enemy : enemies) {
                if (enemy->active && SDL_HasIntersection(&bullet.rect, &enemy->rect)) {
                    enemy->active = false;
                    bullet.active = false;

                    score.addPoints(scoreValue);
                    explosionList.push_back(new Explosion(enemyTexture, enemy->rect.x, enemy->rect.y, 1));
                    SoundManager::playSound("explosion", 0);
                }
            }
        }
    };

    handleBulletWallCollision(player.bullets);
    handleBulletEnemyCollision(player.bullets, 100);
    for (auto& enemy : enemies) {
        handleBulletWallCollision(enemy->bullets);
    }
    if (isTwoPlayerMode && player2 != nullptr) {
        handleBulletWallCollision(player2->bullets);
        handleBulletEnemyCollision(player2->bullets, 100);
    }

    ///Kiểm tra nhà chính
    auto checkBaseHit = [&](std::vector<Bullet>& bullets) {
        for (auto& bullet : bullets) {
            if (bullet.active && SDL_HasIntersection(&bullet.rect, &base->rect)) {
                base->destroy();
                game_over = true;
                return;
            }
        }
    };

    checkBaseHit(player.bullets);
    for (auto& enemy : enemies) {
        checkBaseHit(enemy->bullets);
    }
    if (isTwoPlayerMode && player2 != nullptr) {
        checkBaseHit(player2->bullets);
    }

    ///Update Explosion
    for (auto it = explosionList.begin(); it != explosionList.end(); ) {
        (*it)->update(deltaTime);
        if ((*it)->isFinished()) {
            delete *it;
            it = explosionList.erase(it);
        } else {
            ++it;
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
    base->render(renderer);

    player.render(renderer);

    if (isTwoPlayerMode && player2 != nullptr) {
        player2->render(renderer);
    }

    for(auto &enemy : enemies){
        enemy->render(renderer);
    }

    for (auto& wall : walls) {
        wall->render(renderer);
    }

    for (Explosion* exp : explosionList) {
        exp->render(renderer);
    }
    GameScreen screen(renderer);
    int enemyCount;

    if(currentLevel == 1) enemyCount = enemyCount1;
    else if(currentLevel == 2) enemyCount = enemyCount2;
    else if(currentLevel == 3) enemyCount = enemyCount3;

    int player2Lives = (isTwoPlayerMode && player2 != nullptr) ? player2->lives : MAX_LIVES;

    screen.figures(player.lives, player2Lives, score.getScore(), enemyCount - enemySpawner->getSpawnedCount(), isTwoPlayerMode);

    SDL_RenderPresent(renderer);
}

bool P2_spawned = true;

void Game::run() {
    bool backToMenu = false;
    bool introStage1 = false;
    static bool waitingForTransition = false;
    static Uint32 transitionStartTime = 0;
    GameScreen screen(renderer);

    while (running) {

        if(P2_spawned){
            if (isTwoPlayerMode) {
                player2 = new PlayerTank(10 * TILE_SIZE, 13 * TILE_SIZE, renderer);
                player2->initPlayer2Animations(player2->texture);
                enemySpawner = new EnemySpawner(enemyTexture, enemies, aiControllers, &walls,
                                                player.x, player.y, base->x, base->y, &player, player2);
                P2_spawned = false;
            } else {
                player2 = nullptr;
                enemySpawner = new EnemySpawner(enemyTexture, enemies, aiControllers, &walls,
                                                player.x, player.y, base->x, base->y, &player, nullptr);
                P2_spawned = false;
            }
        }
        handleEvents();

        if(!pause && !game_over){
            update();
            render();
        }
        if(introStage1){
             screen.showStageIntro(renderer, 1);
             introStage1 = false;
        }else if (currentLevel == 1 && enemySpawner->getSpawnedCount() >= enemyCount1 && enemies.empty()) {
            if (!waitingForTransition) {
                waitingForTransition = true;
                transitionStartTime = SDL_GetTicks();
            } else if (SDL_GetTicks() - transitionStartTime >= 4000) {

                screen.showStageIntro(renderer, 2);

                currentLevel = 2;
                resetGame();

                waitingForTransition = false;
            }
        } else if (currentLevel == 2 && enemySpawner->getSpawnedCount() >= enemyCount2 && enemies.empty()) {
            if (!waitingForTransition) {
                waitingForTransition = true;
                transitionStartTime = SDL_GetTicks();
            } else if (SDL_GetTicks() - transitionStartTime >= 4000) {

                screen.showStageIntro(renderer, 3);

                currentLevel = 3;
                resetGame();

                waitingForTransition = false;
            }
        }   else if (currentLevel == 3 && enemySpawner->getSpawnedCount() >= enemyCount3 && enemies.empty()) {
            if (!waitingForTransition) {
                waitingForTransition = true;
                transitionStartTime = SDL_GetTicks();
            } else if (SDL_GetTicks() - transitionStartTime >= 4000) {
                resetGame();
                currentLevel = 1;
                introStage1 = true;

                screen.showVictory(renderer, score.getScore(), backToMenu);
                screen.showMainMenu(renderer, running, isTwoPlayerMode);

                waitingForTransition = false;
            }
        }
        if (game_over) {
            GameScreen screen(renderer);
            backToMenu = false;
            screen.showGameOver(renderer, score.getScore(), backToMenu);

            if (backToMenu) {
                resetGame();
                screen.showMainMenu(renderer, running, isTwoPlayerMode);
                introStage1 = true;

            } else {
                running = false;
            }
        }

        SDL_Delay(16);
    }
}

void Game::resetGame() {
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    for (auto ai : aiControllers) {
        delete ai;
    }
    aiControllers.clear();

    for (auto exp : explosionList) {
        delete exp;
    }
    explosionList.clear();

    for (auto wall : walls) {
        delete wall;
    }
    walls.clear();
    if(currentLevel == 3 && game_over == false){
        generateWalls("map/stage3.txt");
    } else if (currentLevel == 2 && game_over == false) {
        generateWalls("map/stage2.txt");
    } else {
        generateWalls("map/stage1.txt");
    }

    player.respawn();
    player.lives++;
    if (isTwoPlayerMode) {
        player2->respawn();
        player2->lives++;
    }

    if (base != nullptr) {
        delete base;
    }
    base = new Base((MAP_WIDTH / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, "image/base.png");

    if (enemySpawner != nullptr) {
        delete enemySpawner;
    }
    enemySpawner = new EnemySpawner(enemyTexture, enemies, aiControllers, &walls, player.x, player.y, base->x, base->y, &player, nullptr);

    if (game_over == true) {
        currentLevel = 1;
        score.reset();
        player.lives = MAX_LIVES;
        if (isTwoPlayerMode) {
            player2->lives = MAX_LIVES;
        }
    }

    P2_spawned = true;
    game_over = false;
    pause = false;
}
