// this is blatantly disregarding any normal coding conventions
// just doing this because it makes my job a lot shorter
// will look to fix this if i have time

#ifndef GAMEVAR_H
#define GAMEVAR_H

#include <iostream>
#include <array>
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

    /// Purchasable upgrades
    int AttackUpgradeLevel = 0; /// Increases all attack damage by 2% each level, max is 20
    int CritUpgradeLevel = 0; /// Increases crit chance by 1% each level, max is 20
    int DefenseUpgradeLevel = 0; /// Reduces damage taken by 0.5% each level, max is 20
    int EvasionUpgradeLevel = 0; /// Increases evasion chance by 1% each level, max is 20
    int ArmorUpgradeLevel = 0; /// Increases armor effectiveness by 50% per level, max is 5
    int LuckUpgradeLevel = 0; /// Increases player luck for loot drops, monster spawns, etc... Works as a direct addition to randomly rolled values.
    int InventoryUpgradeLevel = 0; /// Increases the number of slots in the inventory, max is 5. 20 - 40 - 60 - 80 - 100 - 200

    float AttackModifier, CritChance, DefenseModifier, EvasionChance, GunAttackModifier, ArmorModifier;

    /// Dungeon info
    bool InDungeon;
    int DungeonMode, DungeonLevel, TurnCount;

    float CurrentHP, MaxHP, CurrentMP, MaxMP;
    // todo: include player status and inventory in here

    // todo: remove this, calculate this in BonusStat in Actor instead
    void CalculateModifiers() {
        AttackModifier = 1.0 + (0.05 * AttackUpgradeLevel) + (0.02 * PlayerLevel);
        CritChance = 0.005 * CritUpgradeLevel + 0.002 * PlayerLevel;
        DefenseModifier = 1.0 - (0.005 * DefenseUpgradeLevel) + 0.002 * PlayerLevel;
        EvasionChance = 0.005 * EvasionUpgradeLevel + 0.002 * PlayerLevel;
        ArmorModifier = 1.0 + (0.5 * ArmorUpgradeLevel) + (0.02 * PlayerLevel);
    }

    int GetInventoryCapacity() {
        return array<int, 6>{20, 40, 60, 80, 100, 200}[InventoryUpgradeLevel];
    }

    void ClearPlayerStats() {
        PlayerLevel = PlayerEXP = CoinBalance = MainGameCompleted = AttackUpgradeLevel = CritUpgradeLevel = DefenseUpgradeLevel = EvasionUpgradeLevel = ArmorUpgradeLevel = LuckUpgradeLevel = InventoryUpgradeLevel = 0;
    }

    void ClearDungeonStats() {
        InDungeon = DungeonMode = DungeonLevel = CurrentHP = CurrentMP = 0;
    }

    void Wipe() {
        GameDataLoaded = 1;
        ClearPlayerStats();
        ClearDungeonStats();
        CalculateModifiers();
    }
};

#endif // GAMEVAR_H
