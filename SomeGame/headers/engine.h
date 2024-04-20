#ifndef ENGINE_H
#define ENGINE_H

#include<bits/stdc++.h>
#include "map.h"
#include "gamevar.h"
#include "actor.h"
#include "layer.h"
#include "rightpanel.h"
using namespace std;

struct Healable {
    string name;
    int sx, sy, amount, cost;
};

struct Monster {
    string name;
    int sx, sy, attack, defense, hp, xp, level;
};

struct Equipment {
    string name;
    int sx, sy, amount, cost, level;
};

const int room_sz[20] = {30, 40, 40, 50, 50, 70, 70, 70, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
const int room_cnt[20] = {5, 10, 10, 15, 15, 20, 20, 20, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
const int monster_cnt[20] = {3, 5, 7, 10, 12, 15, 17, 20, 25, 30, 35, 40, 40, 40, 45, 45, 45, 50, 50, 50};
const int scroll_cnt[20] = {2, 2, 2, 2, 3, 3, 5, 6, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
const int potion_cnt[20] = {2, 2, 2, 4, 5, 5, 6, 6, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

class Engine {
public:
    GameData* gGameData;
    vector<Actor*> actors;
    enum status {
        NotInGame,
        Idle,
        NewTurn,
        Dead
    } GameStatus;
    int Turn = 0, Floor = 1, Mode = -1;
    bool comp = 0;

    vector<Healable> potions;
    vector<Monster> monsters;
    vector<Equipment> weapons;
    vector<Equipment> armors;

    void ImportPotion(vector<Healable>& v);
    void ImportMonsters(vector<Monster>& v);
    void ImportWeapons(vector<Equipment>& v);
    void ImportArmors(vector<Equipment>& v);

    void CreateLevel();
    void WipeLevel();
    void StartGame();

    void EngineSaveGame();
    void EngineLoadGame();

    Engine() {
        gGameData = new GameData;

        ImportPotion(potions);
        ImportMonsters(monsters);
        ImportWeapons(weapons);
        ImportArmors(armors);

        Player = new Actor("Player", 0, 0, 30, 1);
        Player -> combat = new PlayerCombatStat(30, 5, 2, 0);
        Player -> mods = new BonusStat();
        Player -> ai = new PlayerAI();
        Player -> container = new Container(20);
    }

    ~Engine() {
        WipeLevel();
        delete Player;
        delete gGameData;
        delete StatPanel;
        delete inventory;
    }

    void HandleEvent(const SDL_Event* event);
    void Display();

    Actor* Player = nullptr;
    Actor* Stairs = nullptr;
    RightPanel* StatPanel = nullptr;
    DungeonMap* Map = nullptr;
    Layer* Overlay = nullptr;
    Layer* inventory = nullptr;

    int AttackUpgradeLevel = 0;
    int CritUpgradeLevel = 0;
    int DefenseUpgradeLevel = 0;
    int EvasionUpgradeLevel = 0;
    int InventoryUpgradeLevel = 0;

    /// This section contains functions used for purchasable upgrades.
    //{ Attack
    int AttackUpgradeCost() {
        if (AttackUpgradeLevel == 20) return -1;
        return 100 + 20 * AttackUpgradeLevel;
    }

    void AttackUpgrade() {
        ++AttackUpgradeLevel;
    }
    //}
    //{ Defense
    int DefenseUpgradeCost() {
        if (DefenseUpgradeLevel == 20) return -1;
        return 100 + 20 * DefenseUpgradeLevel;
    }

    void DefenseUpgrade() {
        ++DefenseUpgradeLevel;
    }
    //}
    //{ Crit
    int CritUpgradeCost() {
        if (CritUpgradeLevel == 20) return -1;
        return 100 + 20 * CritUpgradeLevel;
    }

    void ArmorUpgrade() {
        ++CritUpgradeLevel;
    }
    //}
    //{ Evasion
    int EvasionUpgradeCost() {
        if (EvasionUpgradeLevel == 20) return -1;
        return 100 + 20 * EvasionUpgradeLevel;
    }

    void LuckUpgrade() {
        ++EvasionUpgradeLevel;
    }

    //}
    //{ Inventory
    int InventoryUpgradeCost() {
        if (InventoryUpgradeLevel == 0) return 300;
        if (InventoryUpgradeLevel == 1) return 500;
        if (InventoryUpgradeLevel == 2) return 800;
        if (InventoryUpgradeLevel == 3) return 1200;
        if (InventoryUpgradeLevel == 4) return 2000;
        return -1;
    }

    void InventoryUpgrade() {
        ++InventoryUpgradeLevel;
        if (InventoryUpgradeLevel == 1) Player -> container -> size = 40;
        if (InventoryUpgradeLevel == 2) Player -> container -> size = 60;
        if (InventoryUpgradeLevel == 3) Player -> container -> size = 90;
        if (InventoryUpgradeLevel == 4) Player -> container -> size = 130;
        if (InventoryUpgradeLevel == 5) Player -> container -> size = 200;
    }
    //}
};

#endif // ENGINE_H
