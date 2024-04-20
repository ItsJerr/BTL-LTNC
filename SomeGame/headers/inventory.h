#ifndef INVENTORY_H
#define INVENTORY_H

#include <bits/stdc++.h>
#include "layer.h"
#include "button.h"
using namespace std;

class Actor;

const int IP = 36;

class Inventory;
class ItemBox {
public:
    ItemBox(Actor* actor);
    ~ItemBox();
    void HandleEvent(const SDL_Event* event, Inventory* owner);
    void Display();

private:
    Actor* actor;
    TextBox* title = nullptr;
    TextBox* use = nullptr;
    TextBox* drop = nullptr;
    TextBox* pointer = nullptr;
    TextBox* desc = nullptr;
    int status = 0;
};

class Inventory: public Layer {
public:
    ItemBox* SelectionBox = nullptr;
    int page, index;

    Inventory(Actor* owner);
    ~Inventory();
    void HandleEvent(const SDL_Event* event) override;
    void Display() override;
private:
    Actor* owner;

    TextBox* title = nullptr;
    TextBox* desc = nullptr;
    TextBox* pointer = nullptr;
    TextBox* items[IP];
};

// a list of names: done
// a pointer that runs through the list: managable: TextBox -> SetPosition
// a function that returns an index when enter is pressed: just keep the index of the position of the pointer?

#endif // INVENTORY_H
