#ifndef LAYER_H
#define LAYER_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "globals.h"

using namespace std;

class EventReceiver {
public:
    virtual ~EventReceiver() {};

    /// EVERYTHING IN A LAYER SHOULD BE AN EventReceiver AND SHOULD HAVE THE FOLLOWING FUNCTIONS:
    virtual bool HandleEvent(const SDL_Event* event) {
        return false;

        /// MUST BE IMPLEMENTED IN MEMBER FUNCTIONS: Returns True if handled, False otherwise.
    }
    virtual void DisplayAsset() {}
};

class Layer {
public:
    virtual ~Layer() {};

    virtual void HandleEvent(const SDL_Event* event) {}

    virtual void Display() {}
};

class Particles: public EventReceiver {
public:
    Particles(SDL_Color col): DotColor(col) {
        status = 0;
        DotColor.a = 0;
		SaveA = col.a;
        delta = max(1, SaveA / 100);
        GenerateDots();
    }

    ~Particles() {
        if (DotLocations) delete[] DotLocations;
    }

    void GenerateDots() {
		if (DotLocations) delete[] DotLocations;
        DotLocations = new SDL_Rect[ParticleCount];

        // Generate random locations for dots
        for (int i = 0; i < ParticleCount; ++i) {
            DotLocations[i] = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 2, 2};
        }
    }
    bool HandleEvent(const SDL_Event* event) override {
        if (event -> type != FrameEventID) return false;

        if (!status) {
            if (DotColor.a > SaveA - delta) status = 1;
            else DotColor.a += delta;
        }
        else {
            DotColor.a -= delta;
            if (DotColor.a == 0) {
                status = 0;
                GenerateDots();
            }
        }

        // cerr << (int)(DotColor.a) << endl;

        return true;
    }
    void DisplayAsset() override {
        SDL_SetRenderDrawColor(gRenderer, DotColor.r, DotColor.g, DotColor.b, DotColor.a);
        for (int i = 0; i < ParticleCount; ++i) SDL_RenderFillRect(gRenderer, &DotLocations[i]);
        assert(gRenderer != nullptr);
    }
private:
    const static int ParticleCount = 1000;
    bool status = 0;
    SDL_Color DotColor;
    Uint8 SaveA = 0, delta = 0;
    SDL_Rect* DotLocations = nullptr;
};

#endif
