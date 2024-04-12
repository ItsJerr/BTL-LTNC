#include "gameplay.h"
#include "engine.h"

gamePlayLayer::gamePlayLayer(){
    Status.push_back(new Stats("Player ", {1300, 20, 2, 2}, 0));
    Status.push_back(new Stats("Experience: ", {1250, 60, 2, 2}, 0));
    Status.push_back(new Stats("Health: ", {1250, 100, 2, 2}, 0));
    Status.push_back(new Stats("Mana: ", {1250, 140, 2, 2}, 0));
    Status.push_back(new Stats("Attack: ", {1250, 180, 2, 2}, 0));
    Status.push_back(new Stats("Defense: ", {1250, 220, 2, 2}, 0));

    Status.push_back(new Stats("Inventory ", {1300, 270, 2, 2}, 0));
    Status.push_back(new Stats("Weapon: ", {1250, 310, 2, 2}, 0));
    Status.push_back(new Stats("Armor: ", {1250, 350, 2, 2}, 0));

    Status.push_back(new Stats("Messages ", {1300, 460, 2, 2}, 0));


    for(Stats* assets : Status) assets -> SetPosition(NULL, 0, 0);
    
}
gamePlayLayer::~gamePlayLayer() {
    for (Stats* asset : Status) delete asset;
    Status.clear();
}



void gamePlayLayer::Display() {
    for (const auto &asset: Status) asset -> DisplayAsset();
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(gRenderer, 1230, 0, 1230, 900);
    SDL_RenderDrawLine(gRenderer, 1230, 260, 1600, 260);
    SDL_RenderDrawLine(gRenderer, 1230, 450, 1600, 450);
    SDL_RenderPresent(gRenderer);
}
