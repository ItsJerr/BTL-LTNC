#include "headers/mainmenu.h"

/// Random particles on the screen that brightens and then dims. Will appear at random locations.
Particles::Particles(SDL_Color col) {
    status = 0;
    DotColor = col;
    GenerateDots();
}

void Particles::GenerateDots() {
    DotLocations = new SDL_Rect[ParticleCount];

    for (int i = 0; i < ParticleCount; ++i) {
        DotLocations[i] = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 2, 2};
    }
}

bool Particles::HandleEvent(const SDL_Event* event) {
    if (event -> type != FrameEventID) return false;

    if (!status) {
        DotColor.a += 3;
        if (DotColor.a == 255) status = 1;
    }
    else {
        DotColor.a -= 3;
        if (DotColor.a == 0) {
            status = 0;
            delete[] DotLocations;
            GenerateDots();
        }
    }

    return true;
}

void Particles::DisplayAsset() {
    SDL_SetRenderDrawColor(gRenderer, DotColor.r, DotColor.g, DotColor.b, DotColor.a);
    for (int i = 0; i < ParticleCount; ++i)
        SDL_RenderFillRect(gRenderer, &DotLocations[i]);
}

//{ Values to assign to button types, just use RNG and const static variables to avoid collision lol
const static int MainMenuModeID = 1000;
//}

//{ Booleans used for handling the display of description buttons
static bool NewGameDescDisplayed = 0;
static bool LoadGameDescDisplayed = 0;
static bool QuitGameDescDisplayed = 0;
//}

/// used to ensure only 1 button is flashing at the same moment
static bool ButtonFlashCheck = 0;

void QuitGameButton::FlashEndAction() {
    SDL_Event tmp;
    tmp.type = SDL_QUIT;
    SDL_PushEvent(&tmp);
}

void NewGameButton::FlashEndAction() {
    CURRENTMODE = NEWGAMEID;
}

void LoadGameButton::FlashEndAction() {
    CURRENTMODE = LOADGAMEID;
}

MainMenuClass::MainMenuClass(int _MODE) {
    MODE = _MODE;

    NewGame = new NewGameButton("new game", {490, 250, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, &ButtonFlashCheck, &NewGameDescDisplayed);
    LoadGame = new LoadGameButton("load game", {490, 331, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, &ButtonFlashCheck, &LoadGameDescDisplayed);
    QuitGame = new QuitGameButton("quit game", {490, 412, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, &ButtonFlashCheck, &QuitGameDescDisplayed);

    NewGameDesc = new TextBox("starts a new game.", {-10, 650, 1300, 75}, white, 3, black, white, &NewGameDescDisplayed);
    LoadGameDesc = new TextBox("loads an existing save file.", {-10, 650, 1300, 75}, white, 3, black, white, &LoadGameDescDisplayed);
    QuitGameDesc = new TextBox("exits the game.", {-10, 650, 1300, 75}, white, 3, black, white, &QuitGameDescDisplayed);

    MainMenuParticle = new Particles({200, 200, 200, 0});

    //{ Add all asset pointers to insiders
    insiders.push_back(MainMenuParticle);
    insiders.push_back(NewGame);
    insiders.push_back(LoadGame);
    insiders.push_back(QuitGame);
    insiders.push_back(NewGameDesc);
    insiders.push_back(LoadGameDesc);
    insiders.push_back(QuitGameDesc);
    //}
}

MainMenuClass::~MainMenuClass() {
    delete NewGame;
    delete LoadGame;
    delete QuitGame;
    delete NewGameDesc;
    delete LoadGameDesc;
    delete QuitGameDesc;
    delete MainMenuParticle;
}

void MainMenuClass::HandleEvent(const SDL_Event* event) {
    if (CURRENTMODE != MODE) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void MainMenuClass::Display() {
    for (const auto &asset: insiders) asset -> DisplayAsset();
}

