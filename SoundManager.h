#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SDL_mixer.h>
#include <map>

#include "defs.h"

class SoundManager {
public:
    static bool init();
    static void loadSound(const std::string& id, const std::string& path);
    static void playSound(const std::string& id, int loops = 0);
    static void freeSound(const std::string& id);
            /// loops = 0: phát 1 lần;  loops = -1: lặp vô hạn
    static void clean();

private:
    static std::map<std::string, Mix_Chunk*> sounds;
};

#endif // SOUND_MANAGER_H
