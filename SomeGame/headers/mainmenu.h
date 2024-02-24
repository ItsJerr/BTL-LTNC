#ifndef MAINMENU_H
#define MAINMENU_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"

/// Random particles on the screen that brightens and then dims. Will appear at random locations.
class Particles: public EventReceiver {
public:
    Particles(SDL_Color col);

    void GenerateDots();
    bool HandleEvent(const SDL_Event* event) override;
    void DisplayAsset() override;
private:
    const static int ParticleCount = 850;
    bool status = 0;
    SDL_Color DotColor;
    SDL_Rect* DotLocations = nullptr;
};

class QuitGameButton: public Button {
public:
    QuitGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor, ScrFlashChk, DispChk) {}

    void FlashEndAction() final;
};

class NewGameButton: public Button {
public:
    NewGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor, ScrFlashChk, DispChk) {}

    void FlashEndAction() final;
};

class LoadGameButton: public Button {
public:
    LoadGameButton(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): Button(msg, _rim, outer, rim_width, inner, _textColor, _flashColor, ScrFlashChk, DispChk) {}

    void FlashEndAction() final;
};

class MainMenuClass: public Layer {
public:
    MainMenuClass(int _MODE);
    ~MainMenuClass();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    int MODE;
    vector<EventReceiver*> insiders;

    NewGameButton* NewGame = nullptr;
    LoadGameButton* LoadGame = nullptr;
    QuitGameButton* QuitGame = nullptr;

    TextBox* NewGameDesc = nullptr;
    TextBox* LoadGameDesc = nullptr;
    TextBox* QuitGameDesc = nullptr;

    Particles* MainMenuParticle = nullptr;
};
#endif // MAINMENU_H
