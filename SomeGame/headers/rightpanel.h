#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include "layer.h"
#include "button.h"

class RightPanel: public Layer {
public:
    void AddMessage(const string& msg);
    RightPanel();
    ~RightPanel();

    void Display() override;
    void HandleEvent(const SDL_Event* event) override {};
private:
    vector<TextBox*> ConstAssets;
    TextBox *Level, *Experience, *Health, *Mana, *Floor, *Turn, *Weapon, *Armor;
    deque<string> MessageList;
};

#endif // RIGHTPANEL_H
