#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"

using namespace std;

class EventReceiver {
public:
/// EVERYTHING IN A LAYER SHOULD BE AN EventReceiver AND SHOULD HAVE THE FOLLOWING FUNCTIONS:
    virtual bool HandleEvent(const SDL_Event* event) {
        return false;

        /// MUST BE IMPLEMENTED IN MEMBER FUNCTIONS: Returns True if handled, False otherwise.
    }
    virtual void DisplayAsset() {}
};

class Layer {
public:
    Layer(int _mode) {
        MODE = _mode;
    }

    bool HandleEvent(const SDL_Event* event) {
        /// Returns True if handled, False otherwise.
        if (MODE != CURRENT_MODE) return 0;
        for (const auto Handler : insiders)
            if (Handler -> HandleEvent(event)) return 1;
        return 0;
    }

    void addAsset(EventReceiver* asset) {
        insiders.push_back(asset);
    }

    void Display() {
        for (const auto asset : insiders)
            asset -> DisplayAsset();
    }
private:
    int MODE;
    vector<EventReceiver*> insiders;
};
