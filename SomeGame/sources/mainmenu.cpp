#include "mainmenu.h"

void Particles::DisplayAsset() {
    // Set color and render dots
    SDL_SetRenderDrawColor(gRenderer, DotColor.r, DotColor.g, DotColor.b, DotColor.a);
    for (int i = 0; i < ParticleCount; ++i)
        SDL_RenderFillRect(gRenderer, &DotLocations[i]);
}

// Booleans used for handling the display of description buttons
static bool NewCharacterDescDisplayed = 0;
static bool LoadCharacterDescDisplayed = 0;
static bool QuitCharacterDescDisplayed = 0;

// Used to ensure only 1 button is flashing at the same moment
static bool ButtonFlashCheck = 0;

void QuitClickAction() {
    // Push SDL_QUIT event to quit the game
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = SDL_QUIT;
    SDL_PushEvent(&tmp);
}

void NewCharacterClickAction() {
    // Push ChangeMode event to change mode
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(NEWCHARACTERID);
    SDL_PushEvent(&tmp);
}

void LoadCharacterClickAction() {
    // Push ChangeMode event to change mode
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(LOADCHARACTERID);
    SDL_PushEvent(&tmp);
}

MainMenuClass::MainMenuClass() {
    // Create buttons and description text boxes
    NewCharacter = new Button("new character", {600, 450, 360, 75}, white, 3, black, white, offwhite, NewCharacterClickAction, &ButtonFlashCheck, &NewCharacterDescDisplayed, 1);
    LoadCharacter = new Button("load character", {600, 531, 360, 75}, white, 3, black, white, offwhite, LoadCharacterClickAction, &ButtonFlashCheck, &LoadCharacterDescDisplayed, 1);
    QuitCharacter = new Button("quit game", {600, 612, 360, 75}, white, 3, black, white, offwhite, QuitClickAction, &ButtonFlashCheck, &QuitCharacterDescDisplayed, 1);

    NewCharacterDesc = new TextBox("create a new character.", {-10, 830, SCREEN_WIDTH + 10, 75}, white, 3, black, white, &NewCharacterDescDisplayed);
    LoadCharacterDesc = new TextBox("load an existing save file.", {-10, 830, SCREEN_WIDTH + 10, 75}, white, 3, black, white, &LoadCharacterDescDisplayed);
    QuitCharacterDesc = new TextBox("exit the game.", {-10, 830, SCREEN_WIDTH + 10, 75}, white, 3, black, white, &QuitCharacterDescDisplayed);

    // Create particles
    MainMenuParticle = new Particles({100, 100, 100, 255});

    // Add all asset pointers to insiders
    insiders.push_back(MainMenuParticle);
    insiders.push_back(NewCharacter);
    insiders.push_back(LoadCharacter);
    insiders.push_back(QuitCharacter);
    insiders.push_back(NewCharacterDesc);
    insiders.push_back(LoadCharacterDesc);
    insiders.push_back(QuitCharacterDesc);
}

MainMenuClass::~MainMenuClass() {
    // Delete all allocated memory for assets
    for (EventReceiver* asset : insiders) delete asset;
    insiders.clear();
}

void MainMenuClass::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    if (CURRENTMODE != MAINMENUID) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void MainMenuClass::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
}
