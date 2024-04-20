#ifndef AI_H
#define AI_H

#include <bits/stdc++.h>
#include "SDL.h"
using namespace std;

class Actor;
class AI {
public:
    virtual void update(Actor* owner, const SDL_Event* event = nullptr) = 0;
};

class PlayerAI: public AI {
public:
    int xpLevel, currentXP;
    int getNextLevelXp();
    void update(Actor* owner, const SDL_Event* event = nullptr) override;

protected:
    bool MoveOrAttack(Actor* owner, int x, int y);
};

class MonsterAI: public AI {
public:
    void update(Actor* owner, const SDL_Event* event = nullptr) override;

protected:
    void MoveOrAttack(Actor* owner, int x, int y);
};

class ConfusedAI: public AI {
public:
    ConfusedAI(int numTurn, AI* oldAI);
    void update(Actor* owner, const SDL_Event* event = nullptr) override;
private:
    int numTurn;
    AI* oldAI;
};

#endif // AI_H
