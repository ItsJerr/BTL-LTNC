#ifndef MAINMENU_H
#define MAINMENU_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"

class MainMenuLayer: public Layer {
public:
    MainMenuLayer();
    ~MainMenuLayer();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    vector<EventReceiver*> insiders;

    bool NewCharacterDescDisplayed = 0, LoadCharacterDescDisplayed = 0, QuitCharacterDescDisplayed = 0, ButtonFlashing = 0;
};
#endif // MAINMENU_H
