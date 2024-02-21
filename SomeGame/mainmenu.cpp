#include "headers/game.h"
#include "headers/globals.h"

namespace MainMenuNamespace {
    Layer MainMenu(MAIN_MENU);

    vector<Button> Buttons;

    void init() {
        Layers.push_back(&MainMenu);

        Buttons.push_back(Button("new game", {490, 282, 300, 75}, white, 3, black, white, {255, 255, 255, 100}));
        Buttons.push_back(Button("load game", {490, 282, 300, 75}, white, 3, black, white, {255, 255, 255, 100}));

        for (unsigned int i = 0; i < Buttons.size(); ++i) MainMenu.addAsset(&Buttons[i]);
    }
}
