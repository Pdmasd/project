#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"
#include "defs.h"
using namespace std;

int main(int argc, char* argv[]) {
    Graphic graphic;
    graphic.initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            break;
        graphic.prepareScene();
        // drawSomething(graphic.window, graphic.renderer);
        SDL_Texture* grass = graphic.loadTexture("image/grass.png");
        int w, h;
        SDL_QueryTexture(grass, NULL, NULL, &w, &h);
        graphic.renderTexture(grass, 100, 100, w, h);
        graphic.presentScene();
        SDL_Delay(100);
    }
}
