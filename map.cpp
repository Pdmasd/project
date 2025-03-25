#include "map.h"

void Map::render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // boundaries
    SDL_RenderClear (renderer); // delete color

    SDL_SetRenderDrawColor (renderer, 0, 0, 0, 255);
    for (int i=1; i < MAP_HEIGHT 1; ++i) {
        for (int j = 1; j < MAP_WIDTH 1; ++j) {
            SDL_Rect tilej TILE_SIZE, i TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect (renderer, &tile);
        }
    }
    SDL_RenderPresent (renderer);
}
