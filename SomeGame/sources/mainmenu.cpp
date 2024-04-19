#include "mainmenu.h"

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
    // Create buttons and description text boxes. Add all asset pointers to insiders
    insiders.push_back(new Particles({100, 100, 100, 255}));
    insiders.push_back(new Button("new character", {600, 450, 360, 75}, 3, NewCharacterClickAction, &ButtonFlashing,
                                  &NewCharacterDescDisplayed));
    insiders.push_back(new Button("load character", {600, 531, 360, 75}, 3, LoadCharacterClickAction, &ButtonFlashing,
                              &LoadCharacterDescDisplayed));
    insiders.push_back(new Button("quit game", {600, 612, 360, 75}, 3, QuitClickAction, &ButtonFlashing,
                              &QuitCharacterDescDisplayed));
    insiders.push_back(new TextBox("create a new character.", {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &NewCharacterDescDisplayed));
    insiders.push_back(new TextBox("load an existing save file.", {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &LoadCharacterDescDisplayed));
    insiders.push_back(new TextBox("exit the game.", {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &QuitCharacterDescDisplayed));
}

MainMenuLayer::~MainMenuLayer() {
    // Delete all allocated memory for assets
    for (EventReceiver* asset : insiders) delete asset;
    insiders.clear();
}

void MainMenuLayer::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void MainMenuLayer::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
}
