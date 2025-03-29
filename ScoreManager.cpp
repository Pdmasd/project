#include "ScoreManager.h"

ScoreManager::ScoreManager() : score(0) {}

ScoreManager::~ScoreManager() {}

void ScoreManager::addPoints(int points) {
    score += points;
}

//void ScoreManager::subtractPoints(int points) {
//    score -= points;
//}

void ScoreManager::reset() {
    score = 0;
}

int ScoreManager::getScore() const {
    return score;
}
