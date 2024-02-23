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
static bool ButtonFlashing = 0;

class QuitGameButton: public Button {
public:
    QuitGameButton(): Button() {}
    QuitGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor) {}

    bool HandleEvent(const SDL_Event* event) final {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered && !ButtonFlashing) {
            inFlash = ButtonFlashing = 1;
            return 1;
        }
        else if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                *DisplayCheck = isHovered;
                return isHovered;
            }
        }
        return false;
    }

    void DisplayAsset() final {
        if (inFlash == 1) {
            if (textSurface) {
                SDL_FreeSurface(textSurface);
                textSurface = nullptr;
            }
            if (textTexture) {
                SDL_DestroyTexture(textTexture);
                textTexture = nullptr;
            }
            /// 3 ticks per sec, flashes for 1 sec
            if ((flashFrameCounter / 10) % 2 == 0)
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), flashColor);
            else
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);

            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            if (++flashFrameCounter == 60) {
                inFlash = ButtonFlashing = 0;
                SDL_Event QuitEvent;
                QuitEvent.type = SDL_QUIT;
                SDL_PushEvent(&QuitEvent);
            }
        }
        else {
            flashFrameCounter = 0;
            textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        }

        /// draw outer rectangle
        SDL_SetRenderDrawColor(gRenderer, rimColor.r, rimColor.g, rimColor.b, rimColor.a);
        SDL_RenderFillRect(gRenderer, &rim);

        /// draw inner rectangle
        SDL_SetRenderDrawColor(gRenderer, centerColor.r, centerColor.g, centerColor.b, centerColor.a);
        SDL_RenderFillRect(gRenderer, &center);

        /// print message
        SDL_RenderCopy(gRenderer, textTexture, nullptr, &centerText);
    }
private:
    bool* DisplayCheck = &QuitGameDescDisplayed;
};

class NewGameButton: public Button {
public:
    NewGameButton(): Button() {}
    NewGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor) {}

    bool HandleEvent(const SDL_Event* event) final {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered && !ButtonFlashing) {
            inFlash = ButtonFlashing = 1;
            return 1;
        }
        else if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                *DisplayCheck = isHovered;
                return isHovered;
            }
        }
        return false;
    }

    void DisplayAsset() final {
        if (inFlash == 1) {
            if (textSurface) {
                SDL_FreeSurface(textSurface);
                textSurface = nullptr;
            }
            if (textTexture) {
                SDL_DestroyTexture(textTexture);
                textTexture = nullptr;
            }
            /// 3 ticks per sec, flashes for 1 sec
            if ((flashFrameCounter / 10) % 2 == 0)
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), flashColor);
            else
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);

            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            if (++flashFrameCounter == 60) {
                inFlash = ButtonFlashing = 0;
                CurrentMode = 2000; /// Choose a save slot
            }
        }
        else {
            flashFrameCounter = 0;
            textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        }

        /// draw outer rectangle
        SDL_SetRenderDrawColor(gRenderer, rimColor.r, rimColor.g, rimColor.b, rimColor.a);
        SDL_RenderFillRect(gRenderer, &rim);

        /// draw inner rectangle
        SDL_SetRenderDrawColor(gRenderer, centerColor.r, centerColor.g, centerColor.b, centerColor.a);
        SDL_RenderFillRect(gRenderer, &center);

        /// print message
        SDL_RenderCopy(gRenderer, textTexture, nullptr, &centerText);
    }
private:
    bool* DisplayCheck = &NewGameDescDisplayed;
};

class LoadGameButton: public Button {
public:
    LoadGameButton(): Button() {}
    LoadGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor) {}

    bool HandleEvent(const SDL_Event* event) final {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered && !ButtonFlashing) {
            inFlash = ButtonFlashing = 1;
            return 1;
        }
        else if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                *DisplayCheck = isHovered;
                return isHovered;
            }
        }
        return false;
    }

    void DisplayAsset() final {
        if (inFlash == 1) {
            if (textSurface) {
                SDL_FreeSurface(textSurface);
                textSurface = nullptr;
            }
            if (textTexture) {
                SDL_DestroyTexture(textTexture);
                textTexture = nullptr;
            }
            /// 3 ticks per sec, flashes for 1 sec
            if ((flashFrameCounter / 10) % 2 == 0)
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), flashColor);
            else
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);

            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            if (++flashFrameCounter == 60) {
                inFlash = ButtonFlashing = 0;
                CurrentMode = 3000; /// Load game screen
            }
        }
        else {
            flashFrameCounter = 0;
            textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        }

        /// draw outer rectangle
        SDL_SetRenderDrawColor(gRenderer, rimColor.r, rimColor.g, rimColor.b, rimColor.a);
        SDL_RenderFillRect(gRenderer, &rim);

        /// draw inner rectangle
        SDL_SetRenderDrawColor(gRenderer, centerColor.r, centerColor.g, centerColor.b, centerColor.a);
        SDL_RenderFillRect(gRenderer, &center);

        /// print message
        SDL_RenderCopy(gRenderer, textTexture, nullptr, &centerText);
    }
private:
    bool* DisplayCheck = &LoadGameDescDisplayed;
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
        if (CurrentMode != MODE) return;

        for (const auto &Handler: insiders) Handler -> HandleEvent(event);
    }

    void Display() override {
        for (const auto &asset: insiders) asset -> DisplayAsset();
    }

private:
    int MODE;
    vector<EventReceiver*> insiders;

    NewGameButton NewGame = NewGameButton("new game", {490, 250, 300, 75}, white, 3, black, white, {255, 255, 255, 100});
    LoadGameButton LoadGame = LoadGameButton("load game", {490, 331, 300, 75}, white, 3, black, white, {255, 255, 255, 100});
    QuitGameButton QuitGame = QuitGameButton("quit game", {490, 412, 300, 75}, white, 3, black, white, {255, 255, 255, 100});

    TextBox NewGameDesc = TextBox("starts a new game.", {-10, 650, 1300, 75}, white, 3, black, white, &NewGameDescDisplayed);
    TextBox LoadGameDesc = TextBox("loads an existing game.", {-10, 650, 1300, 75}, white, 3, black, white, &LoadGameDescDisplayed);
    TextBox QuitGameDesc = TextBox("exits the game.", {-10, 650, 1300, 75}, white, 3, black, white, &QuitGameDescDisplayed);

    Particles MainMenuParticle = Particles({200, 200, 200, 0});
};
