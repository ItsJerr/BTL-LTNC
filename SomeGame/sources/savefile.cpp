#include "savefile.h"
#include <iostream> // For error messages

using namespace std;

// Template function to load a variable from a file
template<typename T>
bool LoadVariableFromFile(ifstream& inFile, T* variable, const string& varName) {
    string variableName;
    // Read variable name from file and compare with expected name
    if (!(inFile >> variableName) || variableName != varName) {
        cerr << variableName << " " << varName << endl;
        cerr << "Error: Failed to load variable '" << varName << "' from file." << endl;
        return false;
    }
    // Read value of the variable from file
    if (!(inFile >> *variable)) {
        cerr << "Error: Failed to read value of variable '" << varName << "' from file." << endl;
        return false;
    }
    return true;
}

// Function to load game data from a save file
void LoadGame(const int& fileIndex, GameData* dataLoc) {
    // Check if game data is already loaded
    if (dataLoc -> GameDataLoaded) {
        cerr << "Error: Game data is already loaded." << endl;
        return;
    }
    dataLoc -> SaveFileIndex = fileIndex;

    // Construct save file path
    string saveFilePath = "assets/saves/savefile" + to_string(fileIndex) + ".sav";
    ifstream loadFileStream(saveFilePath);
    if (!loadFileStream.is_open()) return;

    //{ Define variables to load from the file
                               // Player data
    vector<void*> variables = {&dataLoc -> PlayerLevel, &dataLoc -> PlayerEXP, &dataLoc -> CoinBalance, &dataLoc -> MainGameCompleted,
                               &dataLoc -> MaxHP, &dataLoc -> AttackUpgradeLevel, &dataLoc -> CritUpgradeLevel,
                               &dataLoc -> DefenseUpgradeLevel, &dataLoc -> EvasionUpgradeLevel, &dataLoc -> InventoryUpgradeLevel,
                               // Dungeon data
                               &dataLoc -> InDungeon, &dataLoc -> DungeonMode, &dataLoc -> DungeonLevel, &dataLoc -> TurnCount,
                               &dataLoc -> InventorySize, &dataLoc -> CurrentHP, &dataLoc -> weapon, &dataLoc -> armor};
    //}

    // Load variables from file using the template function
    const vector<string> variableNames = {
        "PlayerLevel", "PlayerEXP", "CoinBalance", "MainGameCompleted",
        "MaxHP", "AttackUpgradeLevel", "CritUpgradeLevel",
        "DefenseUpgradeLevel", "EvasionUpgradeLevel", "InventoryUpgradeLevel",
        "InDungeon", "DungeonMode", "DungeonLevel", "TurnCount",
        "InventorySize", "CurrentHP", "weapon", "armor"
    };

    for (size_t i = 0; i < variableNames.size(); ++i) {
        if (variableNames[i] == "InDungeon" || variableNames[i] == "MainGameCompleted") {
            // Load boolean variables
            if (!LoadVariableFromFile(loadFileStream, static_cast<bool*>(variables[i]), variableNames[i])) {
                loadFileStream.close();
                return;
            }
        }
        else {
            // Load integer variables
            if (!LoadVariableFromFile(loadFileStream, static_cast<int*>(variables[i]), variableNames[i])) {
                loadFileStream.close();
                return;
            }
        }
    }

    for (int i = 0; i < dataLoc -> InventorySize; ++i) {
        string s; int t; loadFileStream >> s >> t;
        dataLoc -> inv.emplace_back(s, t);
    }

    loadFileStream.close();

    // Calculate modifiers and mark game data as loaded
    dataLoc -> CalculateModifiers();
    dataLoc -> GameDataLoaded = true;

    return;
}

// Function to save game data to a save file
void SaveGame(const GameData* dataLoc) {
    // Check if game data is loaded
    if (!dataLoc -> GameDataLoaded) {
        cerr << "Error: Game data is not loaded." << endl;
        return;
    }

    // Construct save file path
    string saveFilePath = "assets/saves/savefile" + to_string(dataLoc -> SaveFileIndex) + ".sav";
    ofstream saveFileStream(saveFilePath);
    if (!saveFileStream.is_open()) {
        cerr << "Error: Failed to open save file '" << saveFilePath << "' for writing." << endl;
        return;
    }

    //{ Save variables to file
    saveFileStream << "PlayerLevel " << dataLoc -> PlayerLevel << endl;
    saveFileStream << "PlayerEXP " << dataLoc -> PlayerEXP << endl;
    saveFileStream << "CoinBalance " << dataLoc -> CoinBalance << endl;
    saveFileStream << "MainGameCompleted " << dataLoc -> MainGameCompleted << endl;
    saveFileStream << "MaxHP " << dataLoc -> MaxHP << endl;
    saveFileStream << "AttackUpgradeLevel " << dataLoc -> AttackUpgradeLevel << endl;
    saveFileStream << "CritUpgradeLevel " << dataLoc -> CritUpgradeLevel << endl;
    saveFileStream << "DefenseUpgradeLevel " << dataLoc -> DefenseUpgradeLevel << endl;
    saveFileStream << "EvasionUpgradeLevel " << dataLoc -> EvasionUpgradeLevel << endl;
    saveFileStream << "InventoryUpgradeLevel " << dataLoc -> InventoryUpgradeLevel << endl;
    saveFileStream << "InDungeon " << dataLoc -> InDungeon << endl;
    saveFileStream << "DungeonMode " << dataLoc -> DungeonMode << endl;
    saveFileStream << "DungeonLevel " << dataLoc -> DungeonLevel << endl;
    saveFileStream << "TurnCount " << dataLoc -> TurnCount << endl;
    saveFileStream << "InventorySize " << dataLoc -> inv.size() << endl;
    saveFileStream << "CurrentHP " << dataLoc -> CurrentHP << endl;
    saveFileStream << "weapon " << dataLoc -> weapon << endl;
    saveFileStream << "armor " << dataLoc -> armor << endl;

    for (auto i : dataLoc -> inv) saveFileStream << i.first << " " << i.second << endl;
    //}

    saveFileStream.close();
    return;
}

// Function to preview the content of a save file
pair<bool, SDL_Texture*> PreviewSaveFile(const int& fileIndex) {
    GameData* TempData = new GameData;

    // Load game data
    LoadGame(fileIndex, TempData);

    // Surface to render text on
    SDL_Surface* TextSurface = SDL_CreateRGBSurface(0, 1050, 650, 32, 0, 0, 0, 0);
    if (TextSurface == nullptr) {
        cerr << "Error: Failed to preview save file: Failed to create a new SDL_Surface.";
        SDL_FreeSurface(TextSurface);
        delete TempData;
        return make_pair(0, nullptr);
    }
    SDL_SetColorKey(TextSurface, SDL_TRUE, SDL_MapRGB(TextSurface -> format, 0, 0, 0));

    // Check whether valid save file exists
    bool ret = 0;

    // Render text based on loaded game data
    if (TempData -> GameDataLoaded) {
        string Description = "";
        Description += "Level: " + to_string(TempData -> PlayerLevel) + "\n";

        Description += "Experience: " + to_string(TempData -> PlayerEXP) + "\n";

        Description += "Balance: " + to_string(TempData -> CoinBalance) + " coin" + ((TempData -> CoinBalance > 1) ? "s" : "") + "\n";

        Description += "Damage dealt +" + to_string((int)(100 * (TempData -> AttackModifier - 1))) + "%\n";
        // this is probably the worst ever coding decision i've made in this code so far.
        // basically, truncates spare digits at the back. can use stringstream but i'm lazy so...
        string stat = to_string(100.0 * TempData -> CritChance);
        while (stat.size() > 1 && (stat.back() == '0' || stat.back() == '.')) stat.pop_back();
        Description += "Crit chance +" + stat + "%\n";

        stat = to_string(100.0 * (1.0 - TempData -> DefenseModifier));
        while (stat.size() > 1 && (stat.back() == '0' || stat.back() == '.')) stat.pop_back();
        Description += "Damage taken -" + stat + "%\n";

        stat = to_string(100.0 * (TempData -> EvasionChance));
        while (stat.size() > 1 && (stat.back() == '0' || stat.back() == '.')) stat.pop_back();
        Description += "Evasion chance +" + stat + "%\n";

        Description += "Inventory Capacity: " + to_string(TempData -> GetInventoryCapacity()) + "\n";

        // Render text lines
        SDL_Surface* LineSurface = TTF_RenderText_Solid_Wrapped(gFont, Description.c_str(), white, 0);
        SDL_BlitSurface(LineSurface, nullptr, TextSurface, nullptr);
        SDL_FreeSurface(LineSurface);

        ret = 1;
    }
    else {
        // Render "<empty>" if game data is not loaded
        SDL_Surface* LineSurface = TTF_RenderText_Solid(gFont, "<empty>", white);
        SDL_Rect DstRect;
        DstRect.w = LineSurface -> w; DstRect.h = LineSurface -> h;
        DstRect.x = (TextSurface -> w - DstRect.w) / 2;
        DstRect.y = (TextSurface -> h - DstRect.h) / 2;

        SDL_BlitSurface(LineSurface, nullptr, TextSurface, &DstRect);
        SDL_FreeSurface(LineSurface);
    }

    // Create texture from surface
    SDL_Texture* TextTexture = SDL_CreateTextureFromSurface(gRenderer, TextSurface);
    SDL_FreeSurface(TextSurface);

    // Delete temporary game data
    delete TempData;

    return make_pair(ret, TextTexture);
}
