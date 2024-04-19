#ifndef GAMELAYER_H
#define GAMELAYER_H

#include "engine.h"
#include "layer.h"

class GameLayer: public Layer {
    void HandleEvent(const SDL_Event* event) override {
        gEngine -> HandleEvent(event);
    }

    void Display() override {
        gEngine -> Display();
    }
};

#endif // GAMELAYER_H
