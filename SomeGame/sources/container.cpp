#include "container.h"
#include "actor.h"

Container::~Container() {
    for (Actor* actor : inventory) delete actor;
    inventory.clear();
}

bool Container::add(Actor* actor) {
    if (inventory.size() >= size) return false;
    inventory.push_back(actor);
    return true;
}

void Container::remove(Actor* actor) {
    auto it = find(inventory.begin(), inventory.end(), actor);
    if (it != inventory.end()) inventory.erase(it);
}
