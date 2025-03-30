#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

class ScoreManager {
private:
    int score;

public:
    ScoreManager();
    ~ScoreManager();

    void addPoints(int points);

    void reset();

    int getScore() const;
    void setScore(int s) { score = s; }
};

#endif // SCOREMANAGER_H
