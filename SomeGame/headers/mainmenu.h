#ifndef MAINMENU_H
#define MAINMENU_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"

class MainMenuClass: public Layer {
public:
    MainMenuClass();
    ~MainMenuClass();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    vector<EventReceiver*> insiders;

    Button* NewCharacter = nullptr;
    Button* LoadCharacter = nullptr;
    Button* QuitCharacter = nullptr;

    TextBox* NewCharacterDesc = nullptr;
    TextBox* LoadCharacterDesc = nullptr;
    TextBox* QuitCharacterDesc = nullptr;

    Particles* MainMenuParticle = nullptr;
};
#endif // MAINMENU_H
