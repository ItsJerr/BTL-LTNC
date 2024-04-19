#ifndef ACTOR_H
#define ACTOR_H

#include <bits/stdc++.h>
#include "combat.h"
#include "ai.h"
#include "pickable.h"
#include "container.h"
using namespace std;

class BonusStat {
public:
    float AttackModifier = 1.0; // damage should be multiplied by this
    float CritChance = 0.0; // a critical hit deals anywhere between 1.3 and 1.8 times more damage
    float DefenseModifier = 1.0; // defense should be multiplied by this
    float EvasionChance = 0.0; // evasion = 0 damage taken
};

class Actor {
public:
    int x, y; // position
    int tilx, tily; // position in the tileset
    string name;
    bool blocks = 1; // whether it blocks the way or not

    BonusStat* mods = nullptr;
    CombatStat* combat = nullptr;
    Pickable* pickable = nullptr;
    Container* container = nullptr;
    AI* ai = nullptr;
    Actor *weapon = nullptr, *armor = nullptr;

    Actor(const string& name, int x, int y, int px, int py);
    ~Actor() {
        if (mods) delete mods;
        if (combat) delete combat;
        if (ai) delete ai;
        if (container) delete container;
        if (pickable) delete pickable;
    }

    void MeleeCombat(Actor* oppo);
    void Display(int relx, int rely);
    void update(const SDL_Event* event = nullptr);
};

#endif // ACTOR_H
