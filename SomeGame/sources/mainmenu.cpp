#include "mainmenu.h"

void Particles::DisplayAsset() {
    // Set color and render dots
    SDL_SetRenderDrawColor(gRenderer, DotColor.r, DotColor.g, DotColor.b, DotColor.a);
    for (int i = 0; i < ParticleCount; ++i)
        SDL_RenderFillRect(gRenderer, &DotLocations[i]);
}

bool QuitClickAction() {
    // Push SDL_QUIT event to quit the game
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = SDL_QUIT;
    SDL_PushEvent(&tmp);
    return 0;
}

bool NewCharacterClickAction() {
    // Push ChangeMode event to change mode
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(NEWCHARACTERID);
    SDL_PushEvent(&tmp);
    return 0;
}

bool LoadCharacterClickAction() {
    // Push ChangeMode event to change mode
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(LOADCHARACTERID);
    SDL_PushEvent(&tmp);
    return 0;
}

MainMenuLayer::MainMenuLayer() {
    // Create buttons and description text boxes
    NewCharacter = new Button("new character", {600, 450, 360, 75}, 3, NewCharacterClickAction, &ButtonFlashing,
                              &NewCharacterDescDisplayed);
    LoadCharacter = new Button("load character", {600, 531, 360, 75}, 3, LoadCharacterClickAction, &ButtonFlashing,
                               &LoadCharacterDescDisplayed);
    QuitCharacter = new Button("quit game", {600, 612, 360, 75}, 3, QuitClickAction, &ButtonFlashing,
                               &QuitCharacterDescDisplayed);

    NewCharacterDesc = new TextBox("create a new character.", {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &NewCharacterDescDisplayed);
    LoadCharacterDesc = new TextBox("load an existing save file.", {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &LoadCharacterDescDisplayed);
    QuitCharacterDesc = new TextBox("exit the game.", {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &QuitCharacterDescDisplayed);

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

MainMenuLayer::~MainMenuLayer() {
    // Delete all allocated memory for assets
    for (EventReceiver* asset : insiders) delete asset;
    insiders.clear();
}

void MainMenuLayer::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    if (CURRENTMODE != MAINMENUID) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void MainMenuLayer::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
}
