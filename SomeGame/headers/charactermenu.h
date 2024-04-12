#ifndef CHARACTERMENU_H
#define CHARACTERMENU_H

#include "button.h"

class CharacterMenuLayer: public Layer {
public:
    CharacterMenuLayer();
    ~CharacterMenuLayer();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    EventReceiver* Buttons[4]; // play classic, play endless, shop, return to main menu
    TextBox* ButtonDescriptions[4];
    bool DisplayControl[5] = {0, 0, 0, 0, 0}; // ButtonFlashing + 3 dependency bools for the 3 text boxes.

    vector<EventReceiver*> insiders;
};

#endif // CHARACTERMENU_H
