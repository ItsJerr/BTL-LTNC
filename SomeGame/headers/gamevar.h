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
    int PlayerLevel, PlayerEXP, CoinBalance;
    /// Every stat increases by 2% per level, max is 50
    bool MainGameCompleted, GunBought;
    int HPUpgradeLevel; /// Increases max HP by 5% each level, max is 50
    int MPUpgradeLevel; /// Increases max MP by 5% each level, max is 50
    int AttackUpgradeLevel; /// Increases all attack damage by 5% each level, max is 50
    int CritUpgradeLevel; /// Increases crit chance by 0.5% each level, max is 50
    int DefenseUpgradeLevel; /// Reduces damage taken by 0.5% each level, max is 50
    int EvasionUpgradeLevel; /// Increases evasion chance by 0.5% each level, max is 50
    int GunUpgradeLevel; /// Increases gun damage by 30% each level, max is 10
    int AmmoUpgradeLevel; /// Increases initial ammo and max ammo, max is 5. 10 - 20 - 35 - 60 - 100 - 200
    int ArmorUpgradeLevel; /// Increases armor effectiveness by 50% per level, max is 5
    int LuckUpgradeLevel; /// Increases player luck for loot drops, monster spawns, etc... Works as a direct addition to randomly rolled values.
    int InventoryUpgradeLevel; /// Increases the number of slots in the inventory, max is 5. 20 - 40 - 60 - 80 - 100 - 200

    float HPModifier, MPModifier, AttackModifier, CritChance, DefenseModifier, EvasionChance, GunAttackModifier, ArmorModifier;

    /// Dungeon info
    bool InDungeon;
    int DungeonMode, DungeonLevel;
    int EnemyHPModifier; /// +2 HP for every monster each 2 level
    float EnemyDamageModifier; /// Increases by 5% every dungeon level
    // todo: include player status and inventory in here

    void CalculateModifiers() {
        HPModifier = 1.0 + (0.05 * HPUpgradeLevel) + (0.02 * PlayerLevel);
        MPModifier = 1.0 + (0.05 * MPUpgradeLevel) + (0.02 * PlayerLevel);
        AttackModifier = 1.0 + (0.05 * AttackUpgradeLevel) + (0.02 * PlayerLevel);
        CritChance = 0.005 * CritUpgradeLevel + 0.002 * PlayerLevel;
        DefenseModifier = 1.0 - (0.005 * DefenseUpgradeLevel) + 0.002 * PlayerLevel;
        EvasionChance = 0.005 * EvasionUpgradeLevel + 0.002 * PlayerLevel;
        GunAttackModifier = 1.0 + (0.3 * GunUpgradeLevel) + (0.02 * PlayerLevel);
        ArmorModifier = 1.0 + (0.5 * ArmorUpgradeLevel) + (0.02 * PlayerLevel);
        EnemyHPModifier = (DungeonLevel - 1) / 2;
        EnemyDamageModifier = 1.0 + (0.05 * DungeonLevel) / 2;
    }

    int GetAmmoCapacity() {
        return array<int, 6>{10, 20, 35, 60, 100, 200}[AmmoUpgradeLevel];
    }

    int GetInventoryCapacity() {
        return array<int, 6>{20, 40, 60, 80, 100, 200}[InventoryUpgradeLevel];
    }

    void Wipe() { /// for the love of god, dont use this anywhere else other than creating a new character. PLEASE NO
        GameDataLoaded = 1;
        PlayerLevel = PlayerEXP = CoinBalance = MainGameCompleted = GunBought = HPUpgradeLevel = MPUpgradeLevel = AttackUpgradeLevel = CritUpgradeLevel = DefenseUpgradeLevel = EvasionUpgradeLevel = GunUpgradeLevel = AmmoUpgradeLevel = ArmorUpgradeLevel = LuckUpgradeLevel = InventoryUpgradeLevel = InDungeon = DungeonMode = DungeonLevel = 0;

        CalculateModifiers();
    }
};

#endif // GAMEVAR_H
