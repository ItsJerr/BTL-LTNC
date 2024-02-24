#ifndef LAYER_H
#define LAYER_H

#include<bits/stdc++.h>
#include "SDL.h"

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

#endif
