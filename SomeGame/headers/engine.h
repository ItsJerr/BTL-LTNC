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
    int Turn = 0;

    vector<Healable> potions;

    void ImportPotion(vector<Healable>& v);

    void CreateLevel();
    void WipeLevel();
    void StartGame();

    Engine() {
        gGameData = new GameData;
        StatPanel = new RightPanel;

        ImportPotion(potions);
    }

    ~Engine() {
        delete gGameData;
        delete StatPanel;
    }

    void HandleEvent(const SDL_Event* event);
    void Display();

    /// This section contains functions used for purchasable upgrades.
    //{ Attack
    int AttackUpgradeCost() {
        if (gGameData -> AttackUpgradeLevel == 50) return -1;
        return int(20 * pow(1.15, gGameData -> AttackUpgradeLevel)) * 5;
    }

    bool AttackUpgrade() {
        gGameData -> CoinBalance -= AttackUpgradeCost();
        ++gGameData -> AttackUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
    //{ Defense
    int DefenseUpgradeCost() {
        if (gGameData -> DefenseUpgradeLevel == 50) return -1;
        return int(20 * pow(1.15, gGameData -> DefenseUpgradeLevel)) * 5;
    }

    bool DefenseUpgrade() {
        gGameData -> CoinBalance -= DefenseUpgradeCost();
        ++gGameData -> DefenseUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
    //{ Armor
    int ArmorUpgradeCost() {
        if (gGameData -> ArmorUpgradeLevel == 5) return -1;
        return array<int, 5>{3000, 8000, 15000, 25000, 50000}[gGameData -> ArmorUpgradeLevel];
    }

    bool ArmorUpgrade() {
        gGameData -> CoinBalance -= ArmorUpgradeCost();
        ++gGameData -> ArmorUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
    //{ Luck
    int LuckUpgradeCost() {
        if (gGameData -> LuckUpgradeLevel == 5) return -1;
        return 30000;
    }

    bool LuckUpgrade() {
        gGameData -> CoinBalance -= LuckUpgradeCost();
        ++gGameData -> LuckUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }

    //}
    //{ Inventory
    int InventoryUpgradeCost() {
        if (gGameData -> InventoryUpgradeLevel == 5) return -1;
        return 10000 * pow(2, gGameData -> InventoryUpgradeLevel);
    }

    bool InventoryUpgrade() {
        gGameData -> CoinBalance -= InventoryUpgradeCost();
        ++gGameData -> InventoryUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}

    Actor* Player = nullptr;
    Actor* Stairs = nullptr;
    RightPanel* StatPanel = nullptr;
    DungeonMap* Map = nullptr;
    Layer* Overlay = nullptr;
};

#endif // ENGINE_H
