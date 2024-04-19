#include "inventory.h"
#include "actor.h"
#include "engine.h"
#include "globals.h"

Inventory::Inventory(vector<Actor*>* inventory) : inventory(inventory), page(0), index(0) {
    title = new TextBox("inventory", {372, 208, 1, 1}, 0);
    title -> SetFont(gameFont);
    title -> SetPosition(nullptr, 0, 0);
    title -> SetColor(transparent, transparent, white);

    desc = new TextBox("arrow up/down: prev/next item\narrow left/right: prev/next page\nx: close\nz: select", {372, 610, 1, 1}, 0);
    desc -> SetFont(gameFont);
    desc -> SetPosition(nullptr, 0, 0);
    desc -> SetColor(transparent, transparent, white);

    pointer = new TextBox(">", {0, 0, 1, 1}, 0);
    pointer -> SetFont(gameFont);
    pointer -> SetPosition(nullptr, 0, 0);
    pointer -> SetColor(transparent, transparent, white);

    for (int i = 0; i < IP; ++i) { // just placeholders until display
        items[i] = new TextBox(" ", {(i < (IP / 2)) ? 385 : 612, 240 + 20 * (i % (IP / 2)), 1, 1}, 0);
        items[i] -> SetFont(gameFont);
        items[i] -> SetPosition(nullptr, 0, 0);
        items[i] -> SetColor(transparent, transparent, white);
    }
}

Inventory::~Inventory() {
    delete title;
    delete desc;
    for (int i = 0; i < IP; ++i) delete items[i];
}

ItemBox::ItemBox(Actor* actor) : actor(actor) {
    title = new TextBox(actor -> name, {485, 380, 1, 1}, 0);
    title -> SetFont(gameFont);
    title -> SetPosition(nullptr, 0, 0);
    title -> SetColor(transparent, transparent, white);

    pointer = new TextBox(">", {473, 405, 1, 1}, 0);
    pointer -> SetFont(gameFont);
    pointer -> SetPosition(nullptr, 0, 0);
    pointer -> SetColor(transparent, transparent, white);

    use = new TextBox("Use", {485, 405, 1, 1}, 0);
    use -> SetFont(gameFont);
    use -> SetPosition(nullptr, 0, 0);
    use -> SetColor(transparent, transparent, white);

    drop = new TextBox("Discard", {485, 425, 1, 1}, 0);
    drop -> SetFont(gameFont);
    drop -> SetPosition(nullptr, 0, 0);
    drop -> SetColor(transparent, transparent, white);

    desc = new TextBox("arrow up/down: move up/down\nx: close\nz: select", {480, 450, 1, 1}, 0);
    desc -> SetFont(gameFont);
    desc -> SetPosition(nullptr, 0, 0);
    desc -> SetColor(transparent, transparent, white);
}

ItemBox::~ItemBox() {
    delete title;
    delete pointer;
    delete use;
    delete drop;
    delete desc;
}

void ItemBox::Display() {
    SDL_SetRenderDrawColor(gRenderer, 230, 230, 230, 255);
    static const SDL_Rect rim = {465, 375, 300, 145};
    SDL_RenderFillRect(gRenderer, &rim);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    static const SDL_Rect inner = {468, 378, 294, 139};
    SDL_RenderFillRect(gRenderer, &inner);

    SDL_SetRenderDrawColor(gRenderer, 230, 230, 230, 255);
    SDL_RenderDrawLine(gRenderer, 465, 400, 763, 400);
    SDL_RenderDrawLine(gRenderer, 465, 450, 763, 450);

    title -> DisplayAsset();
    pointer -> DisplayAsset();
    use -> DisplayAsset();
    drop -> DisplayAsset();
    desc -> DisplayAsset();
}

void ItemBox::HandleEvent(const SDL_Event* event, Inventory* owner) {
    if (event -> type != SDL_KEYDOWN) return;
    switch (event -> key.keysym.sym) {
        case SDLK_DOWN: {
            status = 1;
            static const SDL_Rect pos = {473, 425, 1, 1};
            pointer -> SetPosition(&pos, 0, 0);
            break;
        }
        case SDLK_UP: {
            status = 0;
            static const SDL_Rect pos = {473, 405, 1, 1};
            pointer -> SetPosition(&pos, 0, 0);
            break;
        }
        case SDLK_x: {
            delete owner -> SelectionBox;
            owner -> SelectionBox = nullptr;
            break;
        }
        case SDLK_z: {
            delete owner -> SelectionBox;
            owner -> SelectionBox = nullptr;
            delete gEngine -> Overlay;
            gEngine -> Overlay = nullptr;
            ++gEngine -> Turn;
            if (status == 0) actor -> pickable -> use(actor, gEngine -> Player);
            else gEngine -> Player -> container -> remove(actor);
            gEngine -> GameStatus = Engine::NewTurn;
            break;
        }
    }
}

void Inventory::HandleEvent(const SDL_Event* event) {
    if (event -> type != SDL_KEYDOWN) return;
    if (SelectionBox) {
        SelectionBox -> HandleEvent(event, this);
        return;
    }
    switch (event -> key.keysym.sym) {
        case SDLK_LEFT: {
            if (page > 0) {
                --page;
                index = page * IP;
            }
            break;
        }
        case SDLK_RIGHT: {
            if (IP * (page + 1) < (*inventory).size()) {
                ++page;
                index = page * IP;
            }
            break;
        }
        case SDLK_UP: {
            if (index > page * IP) --index;
            break;
        }
        case SDLK_DOWN: {
            if (index - page * IP < IP - 1 && index + 1 < (*inventory).size()) ++index;
            break;
        }
        case SDLK_x: {
            delete gEngine -> Overlay; // is this safe? idk
            gEngine -> Overlay = nullptr;
            break;
        }
        case SDLK_z: {
            if (index < (*inventory).size()) SelectionBox = new ItemBox((*inventory)[index]);
            break;
        }
    }
}

void Inventory::Display() {
    SDL_SetRenderDrawColor(gRenderer, 230, 230, 230, 255);
    static const SDL_Rect rim = {365, 200, 500, 500};
    SDL_RenderFillRect(gRenderer, &rim);

    SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 255);
    static const SDL_Rect inner = {368, 203, 494, 494};
    SDL_RenderFillRect(gRenderer, &inner);

    SDL_SetRenderDrawColor(gRenderer, 230, 230, 230, 255);
    SDL_RenderDrawLine(gRenderer, 365, 230, 862, 230);
    SDL_RenderDrawLine(gRenderer, 365, 605, 862, 605);

    title -> DisplayAsset();
    desc -> DisplayAsset();

    for (int i = 0; i < IP; ++i) {
        int id = page * IP + i;
        if (id >= (*inventory).size()) break;
        items[i] -> SetText((*inventory)[id] -> name);
        items[i] -> DisplayAsset();
    }

    if (index < (*inventory).size()) {
        int px = index - page * IP;
        SDL_Rect pos = {(px < (IP / 2)) ? 373 : 600, 240 + 20 * (px % (IP / 2)), 1, 1};
        pointer -> SetPosition(&pos, 0, 0);
        pointer -> DisplayAsset();
    }

    if (SelectionBox) SelectionBox -> Display();
}
