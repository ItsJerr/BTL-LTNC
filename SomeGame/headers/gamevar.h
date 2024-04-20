// this is blatantly disregarding any normal coding conventions
// just doing this because it makes my job a lot shorter
// will look to fix this if i have time

#ifndef GAMEVAR_H
#define GAMEVAR_H

#include <bits/stdc++.h>
using namespace std;

const int CLASSICMODE = 100;
const int ENDLESSMODE = 200;

class GameData {
public:
    bool GameDataLoaded = 0;
    int SaveFileIndex = 0;

    /// Player stats
    int PlayerLevel = 0, PlayerEXP = 0, CoinBalance = 0;
    bool MainGameCompleted = 0;
    int MaxHP = 0;

    /// Purchasable upgrades
    int AttackUpgradeLevel = 0; /// Increases all attack damage by 1% each level, max is 20
    int CritUpgradeLevel = 0; /// Increases crit chance by 1% each level, max is 20
    int DefenseUpgradeLevel = 0; /// Reduces damage taken by 0.5% each level, max is 20
    int EvasionUpgradeLevel = 0; /// Increases evasion chance by 0.5% each level, max is 20
    int InventoryUpgradeLevel = 0; /// Increases the number of slots in the inventory, max is 5. 20 - 40 - 60 - 80 - 100 - 200

    /// Dungeon info
    bool InDungeon;
    int DungeonMode, DungeonLevel, TurnCount;

    int CurrentHP, InventorySize;
    int weapon, armor;
    vector<pair<string, int>> inv;
    // todo: include player status and inventory in here

    float AttackModifier, CritChance, EvasionChance, DefenseModifier;
    void CalculateModifiers() {
        AttackModifier = 1.0 + (0.025 * AttackUpgradeLevel) + (0.01 * PlayerLevel);
        CritChance = 0.025 * CritUpgradeLevel + 0.01 * PlayerLevel;
        DefenseModifier = 1.0 - (0.01 * DefenseUpgradeLevel) + 0.005 * PlayerLevel;
        EvasionChance = 0.01 * EvasionUpgradeLevel + 0.005 * PlayerLevel;
    }

    int GetInventoryCapacity() {
        return array<int, 6>{20, 40, 60, 90, 130, 200}[InventoryUpgradeLevel];
    }

    void ClearPlayerStats() {
        PlayerLevel = PlayerEXP = CoinBalance = MainGameCompleted = AttackUpgradeLevel = CritUpgradeLevel = DefenseUpgradeLevel = EvasionUpgradeLevel = InventoryUpgradeLevel = 0;
        MaxHP = 30;
    }

    void ClearDungeonStats() {
        InDungeon = DungeonMode = DungeonLevel = CurrentHP = 0;
    }

    void Wipe() {
        GameDataLoaded = 1;
        ClearPlayerStats();
        ClearDungeonStats();
    }
};

#endif // GAMEVAR_H
