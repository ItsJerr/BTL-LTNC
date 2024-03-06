#ifndef GAMEVAR_H
#define GAMEVAR_H

#include <iostream>
using namespace std;

const int STORYMODE = 100;
const int ENDLESSMODE = 200;

class GameData {
public:
    bool GameDataLoaded = 0;
    int SaveFileIndex = 0;

    /// Player stats
    int PlayerLevel, PlayerEXP, CoinBalance;
    /// Every stat increases by 2% per level, max is 50
    bool GunBought = 0;
    int HPUpgradeLevel; /// Increases max HP by 5% each level, max is 50
    int AttackUpgradeLevel; /// Increases all attack damage by 5% each level, max is 50
    int GunUpgradeLevel; /// Increases gun damage by 30% each level, max is 10
    int ManaUpgradeLevel; /// Increases max MP by 5% each level, max is 50
    int AmmoUpgradeLevel; /// Increases initial ammo and max ammo by 30% each level, max is 5
    int MagicResistanceUpgradeLevel; /// Increases resistance to enemy magic attack by 5% each level, max is 50;
    int ArmorUpgradeLevel; /// Increases armor effectiveness by 50% per level, max is 5

    float HPModifier, DMGModifier, GunDMGModifier, ManaModifier, AmmoModifier, MagicResModifier, ArmorModifier;

    /// Dungeon info
    bool InDungeon;
    int DungeonMode, DungeonLevel;
    int EnemyHPModifier; /// +2 HP for every monster each 2 level
    float EnemyDamageModifier; /// Increases by 5% every dungeon level

    void CalculateModifiers() {
        HPModifier = 1.0 + (0.05 * HPUpgradeLevel) + (0.02 * PlayerLevel);
        DMGModifier = 1.0 + (0.05 * AttackUpgradeLevel) + (0.02 * PlayerLevel);
        GunDMGModifier = 1.0 + (0.3 * GunUpgradeLevel) + (0.02 * PlayerLevel);
        ManaModifier = 1.0 + (0.05 * ManaUpgradeLevel) + (0.02 * PlayerLevel);
        AmmoModifier = 1.0 + (0.3 * AmmoUpgradeLevel) + (0.02 * PlayerLevel);
        ArmorModifier = 1.0 + (0.5 * ArmorUpgradeLevel) + (0.02 * PlayerLevel);
        MagicResModifier = 1.0 + (0.05 * MagicResistanceUpgradeLevel) + (0.02 * PlayerLevel);
        EnemyHPModifier = (DungeonLevel - 1) / 2;
        EnemyDamageModifier = 1.0 + (0.05 * DungeonLevel) / 2;
    }

    void Wipe() { /// for the love of god, dont use this anywhere else other than creating a new character. PLEASE NO
        GameDataLoaded = 1;
        PlayerLevel = PlayerEXP = CoinBalance = GunBought = HPUpgradeLevel = AttackUpgradeLevel = GunUpgradeLevel = ManaUpgradeLevel
        = AmmoUpgradeLevel = MagicResistanceUpgradeLevel = ArmorUpgradeLevel = InDungeon = DungeonMode = DungeonLevel = 0;

        CalculateModifiers();
    }
};

#endif // GAMEVAR_H
