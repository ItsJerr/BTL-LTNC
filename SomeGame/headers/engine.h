#ifndef ENGINE_H
#define ENGINE_H

#include<bits/stdc++.h>
#include "gamevar.h"

using namespace std;

class Engine {
public:
    Engine() {
        gGameData = new GameData;
    }

    ~Engine() {
        delete gGameData;
    }

    GameData* gGameData;

    /// This section contains functions used for purchasable upgrades.
    //{ HP
    int HPUpgradeCost() {
        if (gGameData -> HPUpgradeLevel == 50) return -1;
        return int(20 * pow(1.15, gGameData -> HPUpgradeLevel)) * 5;
    }

    bool HPUpgrade() {
        gGameData -> CoinBalance -= HPUpgradeCost();
        ++gGameData -> HPUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
    //{ MP
    int MPUpgradeCost() {
        if (gGameData -> MPUpgradeLevel == 50) return -1;
        return int(20 * pow(1.15, gGameData -> MPUpgradeLevel)) * 5;
    }

    bool MPUpgrade() {
        gGameData -> CoinBalance -= MPUpgradeCost();
        ++gGameData -> MPUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
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
    //{ Gun purchase
    int GunPurchaseCost() {
        if (gGameData -> GunBought) return -1;
        return 20000;
    }

    bool GunPurchase() {
        gGameData -> CoinBalance -= MPUpgradeCost();
        ++gGameData -> MPUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
    //{ Gun upgrade
    int GunUpgradeCost() {
        if (gGameData -> GunUpgradeLevel == 10) return -1;
        return int(1000 * pow(1.5, gGameData -> GunUpgradeLevel)) * 5;
    }

    bool GunUpgrade() {
        gGameData -> CoinBalance -= GunUpgradeCost();
        ++gGameData -> GunUpgradeLevel;
        gGameData -> CalculateModifiers();
        return false;
    }
    //}
    //{ Ammo
    int AmmoUpgradeCost() {
        if (gGameData -> AmmoUpgradeLevel == 5) return -1;
        return array<int, 5>{3000, 8000, 20000, 45000, 100000}[gGameData -> ArmorUpgradeLevel];
    }

    bool AmmoUpgrade() {
        gGameData -> CoinBalance -= AmmoUpgradeCost();
        ++gGameData -> AmmoUpgradeLevel;
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
};

#endif // ENGINE_H
