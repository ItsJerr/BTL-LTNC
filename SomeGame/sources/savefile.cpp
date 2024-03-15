#include "savefile.h"
#include <iostream> // For error messages

using namespace std;

// Template function to load a variable from a file
template<typename T>
bool LoadVariableFromFile(ifstream& inFile, T* variable, const string& varName) {
    string variableName;
    // Read variable name from file and compare with expected name
    if (!(inFile >> variableName) || variableName != varName) {
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

    // Define variables to load from the file
    vector<void*> variables = {&dataLoc -> PlayerLevel, &dataLoc -> PlayerEXP, &dataLoc -> CoinBalance, &dataLoc -> MainGameCompleted, &dataLoc -> GunBought, &dataLoc -> HPUpgradeLevel, &dataLoc -> AttackUpgradeLevel, &dataLoc -> GunUpgradeLevel, &dataLoc -> ManaUpgradeLevel, &dataLoc -> AmmoUpgradeLevel, &dataLoc -> MagicResistanceUpgradeLevel, &dataLoc -> ArmorUpgradeLevel, &dataLoc -> InDungeon, &dataLoc -> DungeonMode, &dataLoc -> DungeonLevel};

    // Load variables from file using the template function
    const vector<string> variableNames = {
        "PlayerLevel", "PlayerEXP", "CoinBalance", "MainGameCompleted", "GunBought", "HPUpgradeLevel", "AttackUpgradeLevel", "GunUpgradeLevel", "ManaUpgradeLevel", "AmmoUpgradeLevel", "MagicResistanceUpgradeLevel",  "ArmorUpgradeLevel", "InDungeon", "DungeonMode", "DungeonLevel"
    };

    for (size_t i = 0; i < variableNames.size(); ++i) {
        if (variableNames[i] == "GunBought" || variableNames[i] == "InDungeon") {
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

    // Save variables to file
    saveFileStream << "PlayerLevel " << dataLoc -> PlayerLevel << endl;
    saveFileStream << "PlayerEXP " << dataLoc -> PlayerEXP << endl;
    saveFileStream << "CoinBalance " << dataLoc -> CoinBalance << endl;
    saveFileStream << "MainGameCompleted " << dataLoc -> MainGameCompleted << endl;
    saveFileStream << "GunBought " << dataLoc -> GunBought << endl;
    saveFileStream << "HPUpgradeLevel " << dataLoc -> HPUpgradeLevel << endl;
    saveFileStream << "AttackUpgradeLevel " << dataLoc -> AttackUpgradeLevel << endl;
    saveFileStream << "GunUpgradeLevel " << dataLoc -> GunUpgradeLevel << endl;
    saveFileStream << "ManaUpgradeLevel " << dataLoc -> ManaUpgradeLevel << endl;
    saveFileStream << "AmmoUpgradeLevel " << dataLoc -> AmmoUpgradeLevel << endl;
    saveFileStream << "MagicResistanceUpgradeLevel " << dataLoc -> MagicResistanceUpgradeLevel << endl;
    saveFileStream << "ArmorUpgradeLevel " << dataLoc -> ArmorUpgradeLevel << endl;
    saveFileStream << "InDungeon " << dataLoc -> InDungeon << endl;
    saveFileStream << "DungeonMode " << dataLoc -> DungeonMode << endl;
    saveFileStream << "DungeonLevel " << dataLoc -> DungeonLevel << endl;

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
        Description += "Experience: " + to_string(TempData -> PlayerEXP) + "/" + "\n";
        Description += "Balance: " + to_string(TempData -> CoinBalance) + " coin" + ((TempData -> CoinBalance > 1) ? "s" : "") + "\n";
        Description += "Stats:\n";
        Description += "HP +" + to_string((int)(100 * (TempData -> HPModifier - 1))) + "%" + "\n";
        Description += "MP +" + to_string((int)(100 * (TempData -> ManaModifier - 1))) + "%" + "\n";
        Description += "All Damage +" + to_string((int)(100 * (TempData -> DMGModifier - 1))) + "%" + "\n";
        Description += "Armor Effectiveness +" + to_string((int)(100 * (TempData -> ArmorModifier - 1))) + "%" + "\n";
        Description += "Magic Resistance +" + to_string((int)(100 * (TempData -> MagicResModifier - 1))) + "%" + "\n";
        if (TempData -> GunBought) {
            Description += "Additional Gun Damage +" + to_string((int)(100 * (TempData -> DMGModifier - 1))) + "%" + "\n";
            Description += "Maximum Gun Capacity +" + to_string((int)(100 * (TempData -> AmmoModifier - 1))) + "%";
        }

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
