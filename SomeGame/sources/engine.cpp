#include "engine.h"
#include "gamevar.h"
#include "rightpanel.h"
#include "map.h"
#include "actor.h"
#include "inventory.h"

using namespace std;

void Engine::ImportPotion(vector<Healable>& v) {
    ifstream in("assets/data/healables.txt");
    string line, word;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<string> words;
        while (iss >> word) words.push_back(word);

        Healable cur;

        cur.cost = stoi(words.back()); words.pop_back();
        cur.amount = stoi(words.back()); words.pop_back();
        cur.sy = stoi(words.back()); words.pop_back();
        cur.sx = stoi(words.back()); words.pop_back();

        cur.name = "";
        for (const string &w : words) cur.name += w + " ";
        cur.name.pop_back();

        v.push_back(cur);
    }
}

void Engine::WipeLevel() {
    for (Actor* actor: actors) if (actor != Player) delete actor;
    actors.clear();

    if (Map != nullptr) delete Map;

    GameStatus = NotInGame;
}

void Engine::CreateLevel() {
    WipeLevel();
    GameStatus = Idle;

    Map = new DungeonMap(50, 50, 10, 3, 3, 3);

    int stx, sty; tie(stx, sty) = Map -> GetStart();
    Player -> x = stx; Player -> y = sty;

    int edx, edy; tie(edx, edy) = Map -> GetTarget();
    Stairs = new Actor("Stairs", edx, edy, 11, 1);
    Stairs -> blocks = 0;

    Map -> CalculateVisibility(stx, sty);

    actors.push_back(Player);
    actors.push_back(Stairs);
}

void Engine::StartGame() {
    Player = new Actor("Player", 0, 0, 30, 1);
    Player -> combat = new PlayerCombatStat(30, 5, 2);
    Player -> mods = new BonusStat();
    Player -> ai = new PlayerAI();
    Player -> container = new Container(100);
    CreateLevel();
}

void Engine::HandleEvent(const SDL_Event* event) {
    if (GameStatus == Idle) {
        if (Overlay) Overlay -> HandleEvent(event);
        else if (event -> type == SDL_KEYDOWN) {
            if (event -> key.keysym.sym == SDLK_i) Overlay = new Inventory(&Player -> container -> inventory);
            else Player -> update(event);
        }
    }
    if (GameStatus == NewTurn) {
        ++Map -> CurrentScent;
        for (Actor* actor : actors) if (actor != Player && !Player -> combat -> isDead()) actor -> update();
        for (auto it = actors.begin(); it != actors.end();) {
            Actor* actor = *it;
            if (actor != Player && actor -> combat && actor -> combat -> isDead()) {
                delete actor;
                it = actors.erase(it);
            }
            else ++it;
        }
        if (GameStatus == NewTurn) GameStatus = Idle;
    }
}

void Engine::Display() {
    StatPanel -> Display();
    Map -> Display(Player -> x, Player -> y);
    for (Actor* actor : actors) if (actor != Player) if (Map -> IsInFov(actor -> x, actor -> y))
        actor -> Display(actor -> x - Player -> x, actor -> y - Player -> y);
    Player -> Display(0, 0);
    if (Overlay) Overlay -> Display();
}
