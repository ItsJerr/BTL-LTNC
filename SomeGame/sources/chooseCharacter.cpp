#include "chooseCharacter.h"

void commonInit(){
    GameEngine -> gGameData -> GameDataLoaded = 1;
    GameEngine -> gGameData -> PlayerLevel = 0;
    GameEngine -> gGameData -> PlayerEXP = 0;
    GameEngine -> gGameData -> CoinBalance = 0;
    GameEngine -> gGameData -> MainGameCompleted = 0;
    GameEngine -> gGameData -> GunBought = 0;
    GameEngine -> gGameData -> HPUpgradeLevel = 0;
    GameEngine -> gGameData -> AttackUpgradeLevel = 0;
    GameEngine -> gGameData -> GunUpgradeLevel = 0;
    GameEngine -> gGameData -> ManaUpgradeLevel = 0;
    GameEngine -> gGameData -> AmmoUpgradeLevel= 0;
    GameEngine -> gGameData -> MagicResistanceUpgradeLevel = 0;
    GameEngine -> gGameData -> ArmorUpgradeLevel = 0;
    return;
}

bool MageChosen(){
    commonInit();
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(CHARACTERMENUID);
    SDL_PushEvent(&tmp);
    return 0;
}

bool KnightChosen(){
    commonInit();
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(CHARACTERMENUID);
    SDL_PushEvent(&tmp);
    return 0;
}

bool ArcherChosen(){
    commonInit();
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(CHARACTERMENUID);
    SDL_PushEvent(&tmp);
    return 0;
}
bool backToMenu(){
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(MAINMENUID);
    SDL_PushEvent(&tmp);
    return 0;
}
chooseCharacterLayer::chooseCharacterLayer() {
    // Create buttons and description text boxes. Add all asset pointers to insiders
    insiders.push_back(new Particles({100, 100, 100, 255}));
    insiders.push_back(new imageButton("Mage","C:\\Users\\ASUS\\Desktop\\Delete later\\Cap not selected.jpg",
        "C:\\Users\\ASUS\\Desktop\\Delete later\\446e31483a788cb9d0c98e07d28da2f1.jpg",
        {100, 100, 375, 700}, 3, MageChosen, &ButtonFlashing,
                                  &Mage));
    insiders.push_back(new imageButton("Knight","C:\\Users\\ASUS\\Desktop\\Delete later\\Sword not selected.png",
    "C:\\Users\\ASUS\\Desktop\\Delete later\\Sword.png", {625, 100, 375, 700}, 3, KnightChosen, &ButtonFlashing,
                              &Knight));
    insiders.push_back(new imageButton("Archer","C:\\Users\\ASUS\\Desktop\\Delete later\\Bow and arrow not selected.png",
    "C:\\Users\\ASUS\\Desktop\\Delete later\\Bow and arrow.png", {1150, 100, 375, 700}, 3, ArcherChosen, &ButtonFlashing,
                              &Archer));

    insiders.push_back(new Button("X", {1450, 20, 100, 40}, 3, backToMenu, &ButtonFlashing,
                              &Back));

    insiders.push_back(new TextBox("Choose your fucking character", {-10, 830,
        SCREEN_WIDTH + 10, 60}, 3, &chooseYourCharacter));

    insiders.push_back(new TextBox("Gay people which can cast spells.",
        {-10, 830, SCREEN_WIDTH + 10, 60}, 3, &Mage));
    insiders.push_back(new TextBox("Gay people wield swords.",
        {-10, 830, SCREEN_WIDTH + 10, 60}, 3, &Knight));
    insiders.push_back(new TextBox("Gay marksmen with bow and arrow.", {-10, 830,
        SCREEN_WIDTH + 10, 60}, 3, &Archer));
    insiders.push_back(new TextBox("Return to the main menu", {-10, 830,
        SCREEN_WIDTH + 10, 60}, 3, &Back));
}

chooseCharacterLayer::~chooseCharacterLayer() {
    // Delete all allocated memory for assets
    for (EventReceiver* asset : insiders) delete asset;
    insiders.clear();
}
void chooseCharacterLayer::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    if (CURRENTMODE != SELECTIONID) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void chooseCharacterLayer::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
}
