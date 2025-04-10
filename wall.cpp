//wall.cpp
#include "wall.h"

Wall::Wall(int startX, int startY, SDL_Renderer* renderer, const char* texturePath, WallType type)
    : x(startX), y(startY), active(true), type(type) {
    rect = {x, y, TILE_SIZE, TILE_SIZE};

    SDL_Surface* surface = IMG_Load(texturePath);
    if (!surface) {
        SDL_Log("Không thể load ảnh: %s, Lỗi: %s", texturePath, IMG_GetError());
        texture = nullptr;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

Wall::~Wall() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Wall::render(SDL_Renderer* renderer) {
    if (!active || !texture)
        return;

    if (type == WallType::BRICK && rect.w < TILE_SIZE) {

        ///Tọa độ của khối BRICK lớn
        int groupBaseX = (x / TILE_SIZE) * TILE_SIZE;
        int groupBaseY = (y / TILE_SIZE) * TILE_SIZE;
        int smallSize = TILE_SIZE / 4;

        /// Tọa độ pixel nhỏ trong BRICK
        int localX = x - groupBaseX;
        int localY = y - groupBaseY;
        int localCol = localX / smallSize;
        int localRow = localY / smallSize;

        /// Lấy kích thước của texture và tính kích thước mỗi phần
        int texW, texH;
        SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
        int subW = texW / 4;
        int subH = texH / 4;

        ///Mảng texture
        SDL_Rect brickSubRects[16] = {
            { 0,      0,      subW, subH },
            { subW,   0,      subW, subH },
            { 2*subW, 0,      subW, subH },
            { 3*subW, 0,      subW, subH },
            { 0,      subH,   subW, subH },
            { subW,   subH,   subW, subH },
            { 2*subW, subH,   subW, subH },
            { 3*subW, subH,   subW, subH },
            { 0,      2*subH, subW, subH },
            { subW,   2*subH, subW, subH },
            { 2*subW, 2*subH, subW, subH },
            { 3*subW, 2*subH, subW, subH },
            { 0,      3*subH, subW, subH },
            { subW,   3*subH, subW, subH },
            { 2*subW, 3*subH, subW, subH },
            { 3*subW, 3*subH, subW, subH }
        };

        /// Xác định chỉ số texture của pixel
        int index = localRow * 4 + localCol;
        SDL_Rect srcRect = brickSubRects[index];

        SDL_RenderCopy(renderer, texture, &srcRect, &rect);
    }
    else {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

bool Wall::isActive() const {
    return active;
}

void Wall::hit() {
    active = false;
}

std::vector<Wall*> Wall::createBrickGroup(int startX, int startY, SDL_Renderer* renderer, const char* texturePath) {
    std::vector<Wall*> bricks;
    int smallSize = TILE_SIZE / 4;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            int brickX = startX + col * smallSize;
            int brickY = startY + row * smallSize;
            /// Tạo brick mới với type BRICK
            Wall* brick = new Wall(brickX, brickY, renderer, texturePath, WallType::BRICK);
            /// Ghi đè kích thước wall thành smallSize (TILE_SIZE/4)
            brick->rect.w = smallSize;
            brick->rect.h = smallSize;
            bricks.push_back(brick);
        }
    }
    return bricks;
}
