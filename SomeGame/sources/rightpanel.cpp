#include "rightpanel.h"
#include "engine.h"
#include "globals.h"

void RightPanel::AddMessage(const string& msg) {
    MessageList.push_front("> " + msg);
}

RightPanel::RightPanel() {
    // labels
    ConstAssets.push_back(new TextBox("Player", {1250, 0, 0, 24}, 0));
    ConstAssets.push_back(new TextBox("Inventory ", {1250, 169, 0, 24}, 0));
    ConstAssets.push_back(new TextBox("Messages ", {1250, 239, 0, 24}, 0));
    for (TextBox* asset : ConstAssets) {
        asset -> SetFont(gameFont);
        asset -> SetPosition(nullptr, 0, -1);
    }

    // stats
    Level = new TextBox("Level: ", {1240, 20, 0, 24}, 0);
    Level -> SetFont(gameFont);
    Level -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Level -> SetPosition(nullptr, 0, -1);

    Experience = new TextBox("Experience: ", {1240, 45, 0, 24}, 0);
    Experience -> SetFont(gameFont);
    Experience -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Experience -> SetPosition(nullptr, 0, -1);

    Balance = new TextBox("Balance: ", {1240, 70, 0, 24}, 0);
    Balance -> SetFont(gameFont);
    Balance -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Balance -> SetPosition(nullptr, 0, -1);

    Health = new TextBox("Health: ", {1240, 95, 0, 24}, 0);
    Health -> SetFont(gameFont);
    Health -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Health -> SetPosition(nullptr, 0, -1);

    Floor = new TextBox("Floor: ", {1240, 120, 0, 24}, 0);
    Floor -> SetFont(gameFont);
    Floor -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Floor -> SetPosition(nullptr, 0, -1);

    Turn = new TextBox("Turn: ", {1240, 145, 0, 24}, 0);
    Turn -> SetFont(gameFont);
    Turn -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Turn -> SetPosition(nullptr, 0, -1);

    Weapon = new TextBox("Weapon: ", {1240, 190, 0, 24}, 0);
    Weapon -> SetFont(gameFont);
    Weapon -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Weapon -> SetPosition(nullptr, 0, -1);

    Armor = new TextBox("Armor: ", {1240, 215, 0, 24}, 0);
    Armor -> SetFont(gameFont);
    Armor -> SetColor(transparent, transparent, SDL_Color{255, 255, 255, 200});
    Armor -> SetPosition(nullptr, 0, -1);
}

RightPanel::~RightPanel() {
    for (TextBox* asset : ConstAssets) delete asset;
    delete Level;
    delete Experience;
    delete Balance;
    delete Health;
    delete Floor;
    delete Turn;
    delete Weapon;
    delete Armor;
    ConstAssets.clear();
}

void RightPanel::Display() {
    // labels
    for (TextBox* asset : ConstAssets) asset -> DisplayAsset();

    // remaking strings
    Level -> SetText("Level: " + to_string(static_cast<PlayerAI*>(gEngine -> Player -> ai) -> xpLevel));
    Experience -> SetText("Experience: " + to_string(static_cast<PlayerAI*>(gEngine -> Player -> ai) -> currentXP) + "/" + to_string(static_cast<PlayerAI*>(gEngine -> Player -> ai) -> getNextLevelXp()));
    Balance -> SetText("Balance: " + to_string(gEngine -> Player -> balance));
    Health -> SetText("Health: " + to_string(gEngine -> Player -> combat -> HP) + "/" + to_string(gEngine -> Player -> combat -> MaxHP));
    Floor -> SetText("Floor: " + to_string(gEngine -> Floor));
    Turn -> SetText("Turn: " + to_string(gEngine -> Turn));
    if (gEngine -> Player -> weapon) Weapon -> SetText("Weapon: " + gEngine -> Player -> weapon -> name);
    else Weapon -> SetText("Weapon: none");
    if (gEngine -> Player -> armor) Armor -> SetText("Armor: " + gEngine -> Player -> armor -> name);
    else Armor -> SetText("Armor: none");

    // stats
    Level -> DisplayAsset();
    Experience -> DisplayAsset();
    Balance -> DisplayAsset();
    Health -> DisplayAsset();
    Floor -> DisplayAsset();
    Turn -> DisplayAsset();
    Weapon -> DisplayAsset();
    Armor -> DisplayAsset();

    // frame
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(gRenderer, 1230, 10, 1230, 900);
    SDL_RenderDrawLine(gRenderer, 1230, 10, 1245, 10);
    SDL_RenderDrawLine(gRenderer, 1312, 10, 1560, 10);
    SDL_RenderDrawLine(gRenderer, 1230, 180, 1245, 180);
    SDL_RenderDrawLine(gRenderer, 1343, 180, 1560, 180);
    SDL_RenderDrawLine(gRenderer, 1230, 250, 1245, 250);
    SDL_RenderDrawLine(gRenderer, 1331, 250, 1560, 250);

    // messages
    SDL_Surface* MessagesSurface = SDL_CreateRGBSurface(0, 329, 625, 32, 0, 0, 0, 0);
    int py = 0; // height of current text lines
    for (int i = 0; i < MessageList.size(); ++i) {
        SDL_Surface* LineSurface = TTF_RenderText_Solid_Wrapped(gameFont, MessageList[i].c_str(), white, 310);
        if (py + LineSurface -> h > MessagesSurface -> h) { // exceeds the given space
            SDL_FreeSurface(LineSurface);
            while (MessageList.size() > i) MessageList.pop_back(); // pops old messages that won't fit
            break;
        }
        else {
            SDL_Rect DstRect = {10, py, LineSurface -> w, LineSurface -> h};
            SDL_BlitSurface(LineSurface, nullptr, MessagesSurface, &DstRect);
            py += LineSurface -> h + 10;
            SDL_FreeSurface(LineSurface);
        }
    }

    SDL_Texture* MessagesTexture = SDL_CreateTextureFromSurface(gRenderer, MessagesSurface);
    SDL_FreeSurface(MessagesSurface);
    SDL_Rect DstRect = {1231, 270, 329, 625};
    SDL_RenderCopy(gRenderer, MessagesTexture, nullptr, &DstRect);
    SDL_DestroyTexture(MessagesTexture);
}
