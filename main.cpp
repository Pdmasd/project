#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"
#include "defs.h"
#include "shapes.h"
using namespace std;

int main(int argc, char* argv[]) {
    Graphic graphic;
    graphic.initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) break;
        }
        graphic.prepareScene();
        graphic.presentScene();
        SDL_Delay(16);
    }
}
