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
    int Pray = 0; // Pray >= 3 && Break = 0 --> lv 10 boss = The Shadow Assasin
    int Break = 0;// Break >= 3 && Pray = 0 --> lv 10 boss = Rhaast
                  // else lv 10 --> Boss = Kayn - Ghostly Reaper
    int Endless = false;
    bool Kayn = false, Jhin = false;
                  // Jhin simply spawn in horde of 4 kill all 4 to pass
    bool Prayed = false;
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
