#include "layer.h"

class CharacterMenuLayer: public Layer {
public:
    CharacterMenuLayer();
    ~CharacterMenuLayer();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    Button* Buttons[3]; // enter dungeon, shop, return to main menu
    TextBox* ButtonDescriptions[3];
    bool DisplayControl[4] = {0, 0, 0, 0}; // ButtonFlashing + 3 dependency bools for the 3 text boxes.
};
