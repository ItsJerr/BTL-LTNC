#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"

/// Random particles on the screen that brightens and then dims. Will appear at random locations.
class Particles: public EventReceiver {
public:
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

/// values to assign to button types, just use RNG and const static variables to avoid collision lol
const static int NewGameButton = 2431096107;
const static int LoadGameButton = 596889184;

class MainMenuClass: public Layer {
public:
    MainMenuClass(int _MODE) {
        MODE = _MODE;

        insiders.push_back(&MainMenuParticle);
        insiders.push_back(&MainMenu);
        insiders.push_back(&LoadGame);
    }

    bool HandleEvent(const SDL_Event* event) override {
        if (CURRENT_MODE != MODE) return 0;

        for (const auto &Handler: insiders) if (Handler -> HandleEvent(event))
            return 1;

        return 0;
    }

    void Display() override {
        for (const auto &asset: insiders) asset -> DisplayAsset();
    }

private:
    int MODE;
    vector<EventReceiver*> insiders;

    Button MainMenu = Button("new game", {490, 282, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, NewGameButton, true);
    Button LoadGame = Button("load game", {490, 363, 300, 75}, white, 3, black, white, {255, 255, 255, 100}, LoadGameButton, 1);

    Particles MainMenuParticle = Particles({200, 200, 200, 0});
};
