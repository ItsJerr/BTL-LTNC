#include "ai.h"
#include "engine.h"
#include "actor.h"
#include "globals.h"

bool PlayerAI::MoveOrAttack(Actor* owner, int x, int y) {
    if (gEngine -> Map -> IsWall(x, y)) return false;
    ++gEngine -> Turn;
    for (Actor* actor : gEngine -> actors) {
        if(actor -> blocks && actor -> name == "Statue" && actor -> x == x && actor -> y == y){
            gEngine -> StatPanel -> AddMessage("You see the statue of a god here, Kayn - Ghostly Reaper");
            return false;
        }

        if (actor -> blocks && actor -> combat && actor -> x == x && actor -> y == y) {
            owner -> MeleeCombat(actor);
            return false;
        }
    }

    for (Actor* actor : gEngine -> actors) {
        if (!actor -> blocks && actor -> pickable && actor -> x == x && actor -> y == y) {
            gEngine -> StatPanel -> AddMessage("You see a " + actor -> name + " here. Press (z) to pick it up.");
            break;
        }
    }

    if (gEngine -> Stairs -> x == x && gEngine -> Stairs -> y == y)
        gEngine -> StatPanel -> AddMessage("You see a way down here. Press (Enter) to descend.");

    owner -> x = x; owner -> y = y;
    return true;
}

void PlayerAI::update(Actor* owner, const SDL_Event* event) {
    if (owner -> combat -> isDead()) return;
    int dx = 0, dy = 0;
    bool pick = 0, enter = 0, Pray = 0, Break = 0;
    switch (event -> key.keysym.scancode) {
        case SDL_SCANCODE_DOWN: dy = 1; break;
        case SDL_SCANCODE_UP: dy = -1; break;
        case SDL_SCANCODE_RIGHT: dx = 1; break;
        case SDL_SCANCODE_LEFT: dx = -1; break;
        case SDL_SCANCODE_Z: pick = 1; break;
        case SDL_SCANCODE_B: Break = 1; break;
        case SDL_SCANCODE_P: Pray = 1; break;
        case SDL_SCANCODE_RETURN: enter = 1; break;
        default: break;
    }
    if (dx + dy) {
        gEngine -> GameStatus = Engine::NewTurn;
        if (MoveOrAttack(owner, owner -> x + dx, owner -> y + dy))
            gEngine -> Map -> CalculateVisibility(owner -> x, owner -> y);
    }
    else if (pick) {
        ++gEngine -> Turn;
        gEngine -> GameStatus = Engine::NewTurn;

        bool found = 0;
        for (Actor* actor : gEngine -> actors) if (actor -> pickable && actor -> x == owner -> x && actor -> y == owner -> y) {
            found = true;
            if (actor -> pickable -> pick(actor, owner)) {
                gEngine -> StatPanel -> AddMessage("You picked up 1 " + actor -> name + ".");
                break;
            }
            else gEngine -> StatPanel -> AddMessage("Your inventory is full.");
        }
        if (!found) gEngine -> StatPanel -> AddMessage("There's nothing to pick up here...");
    }
    else if (enter) {
        ++gEngine -> Turn;
        if (owner -> x == gEngine -> Stairs -> x && owner -> y == gEngine -> Stairs -> y) {
            gEngine -> StatPanel -> AddMessage("You descended down to floor");
            this -> Prayed = false;
            gEngine -> CreateLevel();
        }
        else {
            gEngine -> GameStatus = Engine::NewTurn;
            gEngine -> StatPanel -> AddMessage("There's no way down here...");
        }
    }
    else if(Pray){
        ++gEngine -> Turn;
        for (Actor* actor : gEngine -> actors) if (actor -> name == "Statue" && abs(actor -> x - owner -> x + actor -> y - owner ->y) <= 1) {
            gEngine -> StatPanel -> AddMessage("You prayed to the god");
            this -> Pray += 1;
            this -> Prayed = true;
            break;
        }
    }
    else if(Break){
        ++gEngine -> Turn;
        for (Actor* actor : gEngine -> actors) if (actor -> name == "Statue" && abs(actor -> x - owner -> x + actor -> y - owner ->y) <= 1) {
            gEngine -> StatPanel -> AddMessage("You destroyed the statue");
            this -> Break += 1;
            auto it = find(gEngine -> actors.begin(), gEngine -> actors.end(), actor);
            if (it != gEngine -> actors.end()) gEngine -> actors.erase(it);
            break;
        }
    }
}

void MonsterAI::MoveOrAttack(Actor* owner, int x, int y) {
    if (gEngine -> Map -> IsWall(x, y)) return;
    for (Actor* actor : gEngine -> actors) {
        if (actor -> blocks && actor -> combat && actor -> x == x && actor -> y == y) {
            owner -> MeleeCombat(actor);
            return;
        }
    }
    owner -> x = x; owner -> y = y;
    return;
}

void MonsterAI::update(Actor* owner, const SDL_Event* event) {
    if (owner -> combat -> isDead()) return;
    int dx = gEngine -> Player -> x - owner -> x;
    int dy = gEngine -> Player -> y - owner -> y;
    float distance = sqrtf(dx * dx + dy * dy);
    if (gEngine -> Map -> IsInFov(owner -> x, owner -> y)) {
        dx = round(1.0 * dx / distance);
        dy = round(1.0 * dy / distance);
        MoveOrAttack(owner, owner -> x + dx, owner -> y + dy);
    }
    else {
        static const int tdx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        static const int tdy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        pair<int, int> best = make_pair(0, -1);
        for (int i = 0; i < 8; ++i) {
            int nx = owner -> x + tdx[i];
            int ny = owner -> y + tdy[i];
            if (gEngine -> Map -> CanWalk(nx, ny)) {
                int cscent = gEngine -> Map -> GetScent(nx, ny);
                if (cscent > gEngine -> Map -> CurrentScent - 20) best = max(best, make_pair(cscent, i));
            }
        }
        if (best.second != -1) MoveOrAttack(owner, owner -> x + tdx[best.second], owner -> y + tdy[best.second]);
    }
}

ConfusedAI::ConfusedAI(int numTurn, AI* oldAI): numTurn(numTurn), oldAI(oldAI) {}

void ConfusedAI::update(Actor* owner, const SDL_Event* event) {
    if (owner == gEngine -> Player) ++gEngine -> Turn;
    int dx = rnd(-1, 1), dy = rnd(-1, 1);
    if (dx + dy) {
        if (gEngine -> Map -> CanWalk(owner -> x + dx, owner -> y + dy)) {
            owner -> x += dx;
            owner -> y += dy;
        }
        else {
            Actor* target = nullptr;
            for (Actor* actor : gEngine -> actors) if (actor -> combat && actor -> x == owner -> x + dx && actor -> y == owner -> y + dy) {
                target = actor;
                break;
            }
            if (target) owner -> MeleeCombat(target);
        }
        gEngine -> GameStatus = Engine::NewTurn;
    }
    if (!--numTurn) {
        owner -> ai = oldAI;
        delete this;
    }
}
