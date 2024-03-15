#include "gamevar.h"

class Engine {
public:
    Engine() {
        gGameData = new GameData;
    }

    ~Engine() {
        delete gGameData;
    }

    GameData* gGameData;
};
