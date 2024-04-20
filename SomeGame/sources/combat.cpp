#include "combat.h"
#include "actor.h"
#include "engine.h"

CombatStat::CombatStat(int maxHP, float attack, float defense, int xp): HP(maxHP), MaxHP(maxHP), attack(attack), defense(defense), xp(xp) {};

int CombatStat::heal(int amount) {
    amount = min(amount, MaxHP - HP);
    HP += amount;
    return amount;
}

void MonsterCombatStat::die(Actor* owner, Actor* killer) {
    if (killer != gEngine -> Player) return;
    gEngine -> StatPanel -> AddMessage(killer -> name + " killed " + owner -> name + "!");
}

void PlayerCombatStat::die(Actor* owner, Actor* killer) {
    gEngine -> StatPanel -> AddMessage("Player died! Killed by " + killer -> name + ".");
    gEngine -> StatPanel -> AddMessage("Press (Enter) to continue...");
    gEngine -> GameStatus = Engine::Dead;
}

void Actor::MeleeCombat(Actor* oppo) {
    float attack = combat -> attack;
    if (weapon) attack += static_cast<Gear*>(weapon -> pickable) -> amount;

    float defense = oppo -> combat -> defense;
    if (oppo -> armor) defense += static_cast<Gear*>(oppo -> armor -> pickable) -> amount;

    float damage;

    if (attack > 2.0 * defense) damage = attack - defense;
    else damage = attack * (0.25 + min(0.25f, 0.125f * max(1.0f, attack / defense)));

    bool crit = 0;

    if (mods) {
        damage *= mods -> AttackModifier;
        if (rround(mods -> CritChance)) {
            damage *= frnd(1.3, 1.8);
            crit = 1;
        }
    }
    if (oppo -> mods) {
        damage *= oppo -> mods -> DefenseModifier;
        if (rround(oppo -> mods -> EvasionChance)) {
            if (name == "Player" || oppo -> name == "Player") {
                gEngine -> StatPanel -> AddMessage(oppo -> name + " evaded the attack from " + name + "!");
                return;
            }
        }
    }

    int rdamage = rround(damage);
    rdamage = min(rdamage, oppo -> combat -> HP);

    if (name == "Player" || oppo -> name == "Player")
        gEngine -> StatPanel -> AddMessage(name + " hit " + oppo -> name + " for " + to_string(rdamage) + " damage!" + (crit ? "!" : ""));

    oppo -> combat -> HP -= damage;
    if (oppo -> combat -> isDead()) {
        balance += oppo -> value;
        if (name == "Player") static_cast<PlayerAI*>(ai) -> currentXP += oppo -> combat -> xp;
        oppo -> combat -> die(oppo, this);
    }
}
