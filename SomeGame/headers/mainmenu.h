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
    const static int ParticleCount = 500;
    bool status = 0;
    SDL_Color DotColor;
    SDL_Rect* DotLocations = nullptr;
};

namespace MainMenuNamespace {
    Layer MainMenu(MAIN_MENU);

    vector<Button> Buttons;
    Particles MainMenuParticles = Particles({150, 150, 150, 0});

    void init() {
        Buttons.push_back(Button("new game", {490, 282, 300, 75}, white, 3, black, white, {255, 255, 255, 100}));
        Buttons.push_back(Button("load game", {490, 363, 300, 75}, white, 3, black, white, {255, 255, 255, 100}));

        MainMenu.addAsset(&MainMenuParticles);
        for (unsigned int i = 0; i < Buttons.size(); ++i) MainMenu.addAsset(&Buttons[i]);
    }
}

