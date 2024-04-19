#include "actor.h"
#include "engine.h"
#include "map.h"

Actor::Actor(const string& name, int x, int y, int px, int py): name(name), x(x), y(y), tilx((px - 1) * 13 + 1), tily((py - 1) * 13 + 1) {}

// coordinates for actors are given relative to the player, as the player will always be at the center of the screen.
const int PlayerX = 29, PlayerY = 21;
void Actor::Display(int relx, int rely) {
    SDL_Rect PositionInTileSet = {tilx, tily, 12, 12};
    SDL_Rect RenderTargetPosition = {(PlayerX + relx) * 21, (PlayerY + rely) * 21, 20, 20};

    SDL_RenderCopy(gRenderer, TileSet, &PositionInTileSet, &RenderTargetPosition);
}

void Actor::update(const SDL_Event* event) {
    if (ai) ai -> update(this, event);
}