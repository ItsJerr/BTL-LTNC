#ifndef COMBAT_H
#define COMBAT_H

#include <bits/stdc++.h>
using namespace std;

class Actor;
class CombatStat {
public:
    int HP, MaxHP, xp;
    float attack, defense;

    CombatStat(int maxHP, float attack, float defense, int xp);

    inline bool isDead() {
        return HP <= 0;
    }
    virtual void die(Actor* owner, Actor* killer) = 0;

    int heal(int amount);
};

class MonsterCombatStat: public CombatStat {
public:
    MonsterCombatStat(int maxHP, float attack, float defense, int xp): CombatStat(maxHP, attack, defense, xp) {};
    void die(Actor* owner, Actor* killer) override;
};

class PlayerCombatStat: public CombatStat {
public:
    PlayerCombatStat(int maxHP, float attack, float defense, int xp): CombatStat(maxHP, attack, defense, xp) {};
    void die(Actor* owner, Actor* killer) override;
};

#endif // COMBAT_H

