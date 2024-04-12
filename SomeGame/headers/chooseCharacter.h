#ifndef CHOOSECHARACTER_H
#define CHOOSECHARACTER_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"

class chooseCharacterLayer: public Layer {
public:
    chooseCharacterLayer();
    ~chooseCharacterLayer();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    vector<EventReceiver*> insiders;

    bool Mage = 0, Knight = 0, Archer = 0, Back = 0,chooseYourCharacter = 1, ButtonFlashing = 0;
};
#endif
