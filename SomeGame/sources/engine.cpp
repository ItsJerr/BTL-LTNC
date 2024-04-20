#include "engine.h"
#include "gamevar.h"
#include "rightpanel.h"
#include "map.h"
#include "actor.h"
#include "inventory.h"
#include "savefile.h"

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

void Engine::ImportMonsters(vector<Monster>& v) {
    ifstream in("assets/data/monsters.txt");
    string line, word;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<string> words;
        while (iss >> word) words.push_back(word);

        Monster cur;

        cur.level = stoi(words.back()); words.pop_back();
        cur.xp = stoi(words.back()); words.pop_back();
        cur.hp = stoi(words.back()); words.pop_back();
        cur.defense = stoi(words.back()); words.pop_back();
        cur.attack = stoi(words.back()); words.pop_back();
        cur.sy = stoi(words.back()); words.pop_back();
        cur.sx = stoi(words.back()); words.pop_back();

        cur.name = "";
        for (const string &w : words) cur.name += w + " ";
        cur.name.pop_back();

        v.push_back(cur);
    }
}

void Engine::ImportWeapons(vector<Equipment>& v) {
    ifstream in("assets/data/weapons.txt");
    string line, word;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<string> words;
        while (iss >> word) words.push_back(word);

        Equipment cur;

        cur.level = stoi(words.back()); words.pop_back();
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

void Engine::ImportArmors(vector<Equipment>& v) {
    ifstream in("assets/data/armors.txt");
    string line, word;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<string> words;
        while (iss >> word) words.push_back(word);

        Equipment cur;

        cur.level = stoi(words.back()); words.pop_back();
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

int sz = 0, room = 0, monster = 0, scroll = 0, potion = 0;

void Engine::CreateLevel() {
    WipeLevel();
    GameStatus = Idle;

    if (Floor < 20) {
        sz = room_sz[Floor], room = room_cnt[Floor], monster = monster_cnt[Floor], scroll = scroll_cnt[Floor], potion = potion_cnt[Floor];
    }
    else if (Mode == ENDLESSMODE) {
        if (sz < 200 && !rnd(0, 4)) {
            sz += 5;
            room += 5;
        }
        if (!rnd(0, 2)) {
            monster = 1.1 * monster;
            scroll += 1;
            potion += 1;
        }
    }
    Map = new DungeonMap(Floor, sz, sz, room, potion, scroll, monster);

    int stx, sty; tie(stx, sty) = Map -> GetStart();
    Player -> x = stx; Player -> y = sty;

    int edx, edy; tie(edx, edy) = Map -> GetTarget();
    Stairs = new Actor("Stairs", edx, edy, 11, 1);
    Stairs -> blocks = 0;

    Map -> CalculateVisibility(stx, sty);

    actors.push_back(Player);
    actors.push_back(Stairs);
}

void Engine::EngineSaveGame() {
    gGameData -> PlayerLevel = static_cast<PlayerAI*>(Player -> ai) -> xpLevel;
    gGameData -> PlayerEXP = static_cast<PlayerAI*>(Player -> ai) -> currentXP;
    gGameData -> CoinBalance = Player -> balance;

    gGameData -> MainGameCompleted = comp;
    gGameData -> MaxHP = Player -> combat -> MaxHP;

    gGameData -> AttackUpgradeLevel = AttackUpgradeLevel;
    gGameData -> DefenseUpgradeLevel = DefenseUpgradeLevel;
    gGameData -> CritUpgradeLevel = CritUpgradeLevel;
    gGameData -> EvasionUpgradeLevel = EvasionUpgradeLevel;
    gGameData -> InventoryUpgradeLevel = InventoryUpgradeLevel;

    gGameData -> InDungeon = ((GameStatus != NotInGame) && (GameStatus != Dead));
    gGameData -> DungeonMode = Mode;
    gGameData -> DungeonLevel = Floor;
    gGameData -> TurnCount = Turn;

    gGameData -> CurrentHP = Player -> combat -> HP;
    gGameData -> InventorySize = Player -> container -> inventory.size();

    if (Player -> weapon == nullptr) gGameData -> weapon = -1;
    else {
        int id = 0;
        for (int i = 0; i < weapons.size(); ++i) if (Player -> weapon -> name == weapons[i].name) {
            id = i;
            break;
        }
        gGameData -> weapon = id;
    }
    if (Player -> armor == nullptr) gGameData -> armor = -1;
    else {
        int id = 0;
        for (int i = 0; i < armors.size(); ++i) if (Player -> armor -> name == armors[i].name) {
            id = i;
            break;
        }
        gGameData -> armor = id;
    }

    gGameData -> inv.clear();
    for (Actor* actor: Player -> container -> inventory) {
        if (typeid(*(actor -> pickable)) == typeid(Healer)) {
            int id = 0;
            for (int i = 0; i < potions.size(); ++i) if (actor -> name == potions[i].name) {
                id = i;
                break;
            }
            gGameData -> inv.push_back(make_pair("healable", id));
        }
        else if (typeid(*(actor -> pickable)) == typeid(Gear)) {
            if (static_cast<Gear*>(actor -> pickable) -> type == 0) {
                int id = 0;
                for (int i = 0; i < weapons.size(); ++i) if (actor -> name == weapons[i].name) {
                    id = i;
                    break;
                }
                gGameData -> inv.push_back(make_pair("weapon", id));
            }
            else {
                int id = 0;
                for (int i = 0; i < armors.size(); ++i) if (actor -> name == armors[i].name) {
                    id = i;
                    break;
                }
                gGameData -> inv.push_back(make_pair("armor", id));
            }
        }
        else if (typeid(*(actor -> pickable)) == typeid(Confuser)) {
            gGameData -> inv.push_back(make_pair("confuser", 0));
        }
        else if (typeid(*(actor -> pickable)) == typeid(Fireball)) {
            gGameData -> inv.push_back(make_pair("fireball", 0));
        }
    }

    SaveGame(gGameData);
}

void Engine::EngineLoadGame() {
    static_cast<PlayerAI*>(Player -> ai) -> xpLevel = gGameData -> PlayerLevel;
    static_cast<PlayerAI*>(Player -> ai) -> currentXP - gGameData -> PlayerEXP;
    Player -> balance = gGameData -> CoinBalance;
    Player -> combat -> attack += 0.5 * gGameData -> PlayerLevel;
    Player -> combat -> defense += 0.5 * gGameData -> PlayerLevel;

    comp = gGameData -> MainGameCompleted;
    Player -> combat -> MaxHP = gGameData -> MaxHP;

    AttackUpgradeLevel = gGameData -> AttackUpgradeLevel;
    DefenseUpgradeLevel = gGameData -> DefenseUpgradeLevel;
    CritUpgradeLevel = gGameData -> CritUpgradeLevel;
    EvasionUpgradeLevel = gGameData -> EvasionUpgradeLevel;
    InventoryUpgradeLevel = gGameData -> InventoryUpgradeLevel;

    if (!gGameData -> InDungeon) return;

    Player -> combat -> HP = gGameData -> CurrentHP;
    Floor = gGameData -> DungeonLevel;
    Turn = gGameData -> TurnCount;
    Mode = gGameData -> DungeonMode;

    if (gGameData -> weapon != -1) {
        int id = gGameData -> weapon;
        Player -> weapon = new Actor(weapons[id].name, 0, 0, weapons[id].sx, weapons[id].sy);
        Player -> weapon -> blocks = 0;
        Player -> weapon -> pickable = new Gear(0, weapons[id].amount);
    }

    if (gGameData -> armor != -1) {
        int id = gGameData -> armor;
        Player -> armor = new Actor(armors[id].name, 0, 0, armors[id].sx, armors[id].sy);
        Player -> armor -> blocks = 0;
        Player -> armor -> pickable = new Gear(1, armors[id].amount);
    }

    for (const pair<string, int>& item : gGameData -> inv) {
        string type; int id; tie(type, id) = item;
        if (type == "healable") {
            Player -> container -> inventory.push_back(new Actor(potions[id].name, 0, 0, potions[id].sx, potions[id].sy));
            Player -> container -> inventory.back() -> blocks = 0;
            Player -> container -> inventory.back() -> pickable = new Healer(potions[id].amount, potions[id].cost);
        }
        else if (type == "weapon") {
            Player -> container -> inventory.push_back(new Actor(weapons[id].name, 0, 0, weapons[id].sx, weapons[id].sy));
            Player -> container -> inventory.back() -> blocks = 0;
            Player -> container -> inventory.back() -> pickable = new Gear(0, weapons[id].amount);
        }
        else if (type == "armor") {
            Player -> container -> inventory.push_back(new Actor(armors[id].name, 0, 0, armors[id].sx, armors[id].sy));
            Player -> container -> inventory.back() -> blocks = 0;
            Player -> container -> inventory.back() -> pickable = new Gear(1, armors[id].amount);
        }
        else if (type == "fireball") {
            Player -> container -> inventory.push_back(new Actor("Scroll of Fireball", 0, 0, 1, 34));
            Player -> container -> inventory.back() -> blocks = 0;
            Player -> container -> inventory.back() -> pickable = new Fireball();
        }
        else if (type == "confuser") {
            Player -> container -> inventory.push_back(new Actor("Scroll of Fireball", 0, 0, 30, 25));
            Player -> container -> inventory.back() -> blocks = 0;
            Player -> container -> inventory.back() -> pickable = new Confuser();
        }
    }

    cerr << "preloading ok\n";

    StartGame();
}

void Engine::StartGame() {
    inventory = new Inventory(Player);
    StatPanel = new RightPanel;
    Player -> combat -> HP = Player -> combat -> MaxHP;
    CreateLevel();
}

void Engine::HandleEvent(const SDL_Event* event) {
    if (GameStatus == Dead) {
        if (event -> type == SDL_KEYDOWN && event -> key.keysym.scancode == SDL_SCANCODE_RETURN) {
            WipeLevel();
            if (Player -> weapon) delete Player -> weapon; Player -> weapon = nullptr;
            if (Player -> armor) delete Player -> armor; Player -> armor = nullptr;
            delete inventory;
            delete StatPanel;

            SDL_Event tmp; SDL_zero(tmp);
            tmp.type = ChangeModeEventID;
            tmp.user.data1 = new int(CHARACTERMENUID);
            SDL_PushEvent(&tmp);
        }
    }
    if (GameStatus == Idle) {
        if (Overlay) Overlay -> HandleEvent(event);
        else if (event -> type == SDL_KEYDOWN) {
            if (event -> key.keysym.sym == SDLK_i) {
                static_cast<Inventory*>(inventory) -> page = static_cast<Inventory*>(inventory) -> index = 0;
                Overlay = inventory;
            }
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
    if (Map == nullptr) return;
    StatPanel -> Display();
    Map -> Display(Player -> x, Player -> y);
    for (Actor* actor : actors) if (actor != Player) if (Map -> IsInFov(actor -> x, actor -> y))
        actor -> Display(actor -> x - Player -> x, actor -> y - Player -> y);
    Player -> Display(0, 0);
    if (Overlay) Overlay -> Display();
}
