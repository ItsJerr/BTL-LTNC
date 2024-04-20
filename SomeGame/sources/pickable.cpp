#include "pickable.h"
#include "actor.h"
#include "globals.h"
#include "engine.h"

bool Pickable::pick(Actor* owner, Actor* wearer) {
    if (wearer -> container && wearer -> container -> add(owner)) {
        auto it = find(gEngine -> actors.begin(), gEngine -> actors.end(), owner);
        if (it != gEngine -> actors.end()) gEngine -> actors.erase(it);
        return true;
    }
    return false;
}

bool Pickable::use(Actor* owner, Actor* wearer) {
    if (wearer -> container) {
        wearer -> container -> remove(owner);
        delete owner;
        return true;
    }
    return false;
}

// implement items here
Gear::Gear(bool type, int value): type(type), amount(value) {};

bool Gear::use(Actor* owner, Actor* wearer) {
    if (wearer -> combat) {
        if (type == 0) { // weapon
            if (wearer -> weapon) {
                if (wearer -> container)
                    wearer -> container -> inventory.push_back(wearer -> weapon); // move the current weapon into the inventory
            }
            wearer -> weapon = owner;
        }
        else { // armor
            if (wearer -> armor) {
                if (wearer -> container)
                    wearer -> container -> inventory.push_back(wearer -> armor); // move the current armor into the inventory
            }
            wearer -> armor = owner;
        }
        if (wearer -> container) {
            wearer -> container -> remove(owner);
            return true;
        }
        return false;
    }
    return false;
}

bool Healer::use(Actor* owner, Actor* wearer) {
    if (wearer -> combat) {
        int healed = wearer -> combat -> heal(amount);
        if (wearer -> name == "Player") {
            if (healed > 0) gEngine -> StatPanel -> AddMessage("You used 1 " + owner -> name + " and healed " + to_string(healed) + " HP.");
            else gEngine -> StatPanel -> AddMessage("You used 1 " + owner -> name + ", but it had no effect.");
        }
        if (healed > 0) {
            return Pickable::use(owner, wearer);
        }
    }
    return false;
}

bool Fireball::use(Actor* owner, Actor* wearer) {
    int targetx = wearer -> x, targety = wearer -> y;

    gEngine -> StatPanel -> AddMessage("Choose a target. Use the arrow keys to move, (x) to cancel and (z) to select.");

    SDL_Event event;
    bool done = 0;
    while (!done) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) exit(0);
        else if (event.type == SDL_KEYDOWN) {
            int dx = 0, dy = 0;
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_DOWN: dy = 1; break;
                case SDL_SCANCODE_UP: dy = -1; break;
                case SDL_SCANCODE_LEFT: dx = -1; break;
                case SDL_SCANCODE_RIGHT: dx = 1; break;
                case SDL_SCANCODE_Z: done = 1; break;
                case SDL_SCANCODE_X: return false;
            }
            if (dx + dy) {
                if (!gEngine -> Map -> IsWall(targetx + dx, targety + dy) && gEngine -> Map -> IsInFov(targetx + dx, targety + dy)) {
                    targetx += dx;
                    targety += dy;
                }
            }
        }

        assert(gEngine -> Overlay == nullptr);
        gEngine -> Display();
        for (int i = -1; i <= 1; ++i) for (int j = -1; j <= 1; ++j) {
            int tx = targetx + i, ty = targety + j;
            if (gEngine -> Map -> IsWall(tx, ty) || !gEngine -> Map -> IsInFov(tx, ty)) continue;
            SDL_Rect RenderTargetPosition = {(29 + tx - gEngine -> Player -> x) * 21, (21 + ty - gEngine -> Player -> y) * 21, 20, 20};
            SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
            SDL_RenderFillRect(gRenderer, &RenderTargetPosition);
        }
        SDL_RenderPresent(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
        SDL_RenderClear(gRenderer);
    }

    gEngine -> StatPanel -> AddMessage("The fireball explodes, burning everything within its range!");
    for (int i = -1; i <= 1; ++i) for (int j = -1; j <= 1; ++j) {
        int tx = targetx + i, ty = targety + j;
        for (Actor* actor : gEngine -> actors) if (actor -> x == tx && actor -> y == ty && actor -> combat) {
            actor -> combat -> HP -= 10;
            gEngine -> StatPanel -> AddMessage("The fireball burned " + actor -> name + " for 10 HP.");
        }
    }
    return Pickable::use(owner, wearer);
}

bool Confuser::use(Actor* owner, Actor* wearer) {
    int targetx = wearer -> x, targety = wearer -> y;

    gEngine -> StatPanel -> AddMessage("Choose a target. Use the arrow keys to move, (x) to cancel and (z) to select.");

    SDL_Event event;
    bool done = 0;
    while (!done) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) exit(0);
        else if (event.type == SDL_KEYDOWN) {
            int dx = 0, dy = 0;
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_DOWN: dy = 1; break;
                case SDL_SCANCODE_UP: dy = -1; break;
                case SDL_SCANCODE_LEFT: dx = -1; break;
                case SDL_SCANCODE_RIGHT: dx = 1; break;
                case SDL_SCANCODE_Z: done = 1; break;
                case SDL_SCANCODE_X: return false;
            }
            if (dx + dy) {
                if (!gEngine -> Map -> IsWall(targetx + dx, targety + dy) && gEngine -> Map -> IsInFov(targetx + dx, targety + dy)) {
                    targetx += dx;
                    targety += dy;
                }
            }
        }

        assert(gEngine -> Overlay == nullptr);
        gEngine -> Display();

        if (gEngine -> Map -> IsWall(targetx, targety) || !gEngine -> Map -> IsInFov(targetx, targety)) continue;
        SDL_Rect RenderTargetPosition = {(29 + targetx - gEngine -> Player -> x) * 21, (21 + targety - gEngine -> Player -> y) * 21, 20, 20};
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(gRenderer, &RenderTargetPosition);

        SDL_RenderPresent(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
        SDL_RenderClear(gRenderer);
    }

    for (Actor* actor : gEngine -> actors) if (actor -> x == targetx && actor -> y == targety && actor -> ai) {
        actor -> ai = new ConfusedAI(15, actor -> ai);
        gEngine -> StatPanel -> AddMessage("The eyes of the " + actor -> name + " looks vacant, " + (actor -> name == "Player" ? "he" : "it") + " starts wandering around!");
        return Pickable::use(owner, wearer);
    }

    gEngine -> StatPanel -> AddMessage("There is no one here...");
    return false;
}

bool Coin::pick(Actor* owner, Actor* wearer) {
    auto it = find(gEngine -> actors.begin(), gEngine -> actors.end(), owner);
    if (it != gEngine -> actors.end()) gEngine -> actors.erase(it);
    ++wearer -> balance;
    return true;
}
