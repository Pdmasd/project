#include "SoundManager.h"
#include "defs.h"

map <string, Mix_Chunk*> SoundManager::sounds;

bool SoundManager::init() {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }
    return true;
}

void SoundManager::loadSound(const std::string& id, const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (chunk == nullptr) {
        SDL_Log("Failed to load sound: %s, error: %s", path.c_str(), Mix_GetError());
    } else {
        sounds[id] = chunk;
    }
}

void SoundManager::playSound(const std::string& id, int loops) {
    if (sounds.find(id) != sounds.end()) {
        Mix_PlayChannel(-1, sounds[id], loops);
    } else {
        SDL_Log("Sound with id '%s' not found!", id.c_str());
    }
}

void SoundManager::freeSound(const std::string& id){
    if (sounds.find(id) != sounds.end()) {
        Mix_FreeChunk(sounds[id]);
        sounds.erase(id);
    } else {
        SDL_Log("Sound with id '%s' not found for free!", id.c_str());
    }
}

void SoundManager::clean() {
    for (auto& soundPair : sounds) {
        Mix_FreeChunk(soundPair.second);
    }
    sounds.clear();
    Mix_CloseAudio();
}
