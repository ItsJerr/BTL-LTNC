#ifndef COMBAT_H
#define COMBAT_H

#include <bits/stdc++.h>
using namespace std;

class Actor;
class CombatStat {
public:
    int HP, MaxHP;
    float attack, defense;

    CombatStat(int maxHP, float attack, float defense);

    inline bool isDead() {
        return HP <= 0;
    }
    virtual void die(Actor* owner, const string& killer) = 0;

    int heal(int amount);
};

class MonsterCombatStat: public CombatStat {
public:
    MonsterCombatStat(int maxHP, float attack, float defense): CombatStat(maxHP, attack, defense) {};
    void die(Actor* owner, const string& killer) override;
};

class PlayerCombatStat: public CombatStat {
public:
    PlayerCombatStat(int maxHP, float attack, float defense): CombatStat(maxHP, attack, defense) {};
    void die(Actor* owner, const string& killer) override;
};

#endif // COMBAT_H

