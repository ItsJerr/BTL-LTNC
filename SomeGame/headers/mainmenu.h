#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"

/// Random particles on the screen that brightens and then dims. Will appear at random locations.
class Particles: public EventReceiver {
public:
    const int type = 0;

    void GenerateDots() {
        DotLocations = new SDL_Rect[ParticleCount];

        for (int i = 0; i < ParticleCount; ++i) {
            DotLocations[i] = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 2, 2};
        }
    }

    Particles(SDL_Color col) {
        status = 0;
        DotColor = col;
        GenerateDots();
    }

    bool HandleEvent(const SDL_Event* event) override {
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

    void DisplayAsset() override {
        SDL_SetRenderDrawColor(gRenderer, DotColor.r, DotColor.g, DotColor.b, DotColor.a);
        for (int i = 0; i < ParticleCount; ++i)
            SDL_RenderFillRect(gRenderer, &DotLocations[i]);
    }
private:
    const static int ParticleCount = 850;
    bool status = 0;
    SDL_Color DotColor;
    SDL_Rect* DotLocations = nullptr;
};

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

class QuitGameButton: public Button {
public:
    QuitGameButton(): Button() {}
    QuitGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor, ScrFlashChk, DispChk) {}

    void FlashEndAction() final {
        SDL_Event tmp;
        tmp.type = SDL_QUIT;
        SDL_PushEvent(&tmp);
    }
};

class NewGameButton: public Button {
public:
    NewGameButton(): Button() {}
    NewGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor, ScrFlashChk, DispChk) {}

    void FlashEndAction() final {
        CURRENTMODE = NEWGAMEID;
    }
};

class LoadGameButton: public Button {
public:
    LoadGameButton(): Button() {}
    LoadGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor, ScrFlashChk, DispChk) {}

    void FlashEndAction() final {
        CURRENTMODE = LOADGAMEID;
    }
};

class MainMenuClass: public Layer {
public:
    MainMenuClass(int _MODE) {
        MODE = _MODE;

        //{ Add all asset pointers to insiders
        insiders.push_back(&MainMenuParticle);
        insiders.push_back(&NewGame);
        insiders.push_back(&LoadGame);
        insiders.push_back(&QuitGame);
        insiders.push_back(&NewGameDesc);
        insiders.push_back(&LoadGameDesc);
        insiders.push_back(&QuitGameDesc);
        //}
    }

    void HandleEvent(const SDL_Event* event) override {
        if (CURRENTMODE != MODE) return;

        for (const auto &Handler: insiders) Handler -> HandleEvent(event);
    }

    void Display() override {
        for (const auto &asset: insiders) asset -> DisplayAsset();
    }

private:
    int MODE;
    vector<EventReceiver*> insiders;

    NewGameButton NewGame = NewGameButton("new game", {490, 250, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, &ButtonFlashCheck, &NewGameDescDisplayed);
    LoadGameButton LoadGame = LoadGameButton("load game", {490, 331, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, &ButtonFlashCheck, &LoadGameDescDisplayed);
    QuitGameButton QuitGame = QuitGameButton("quit game", {490, 412, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, &ButtonFlashCheck, &QuitGameDescDisplayed);

    TextBox NewGameDesc = TextBox("starts a new game.", {-10, 650, 1300, 75}, white, 3, black, white, &NewGameDescDisplayed);
    TextBox LoadGameDesc = TextBox("loads an existing save file.", {-10, 650, 1300, 75}, white, 3, black, white, &LoadGameDescDisplayed);
    TextBox QuitGameDesc = TextBox("exits the game.", {-10, 650, 1300, 75}, white, 3, black, white, &QuitGameDescDisplayed);

    Particles MainMenuParticle = Particles({200, 200, 200, 0});
};
