#ifndef ENGINE_H
#define ENGINE_H

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

#endif // ENGINE_H
