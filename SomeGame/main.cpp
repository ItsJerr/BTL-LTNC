#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "headers/globals.h"
#include "headers/button.h"
using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

ofstream Log("log.txt");

void logSDLError(const std::string &msg, bool fatal = false) {
    Log << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void logTTFError(const std::string &msg, bool fatal = false) {
    Log << msg << " Error: " << TTF_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

/// GLOBAL DISPLAY VARIABLES

void RenderFrame() {
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
}
///

void Cleanup() { /// CALL BEFORE QUITTING
    SDL_DestroyTexture(gTexture); gTexture = nullptr;
    SDL_DestroyRenderer(gRenderer); gRenderer = nullptr;
    SDL_DestroyWindow(gWindow); gWindow = nullptr;

    Log.close();
}

signed main(int argc, char *argv[]) {
    /// Just initialize everything for now, will initilize parts if sluggish
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError("SDL_Init", 1);
    if (TTF_Init() != 0) logTTFError("TTF_Init", 1);

    gFont = TTF_OpenFont("assets/fonts/nasalization-rg.ttf", 50);

    int textWidth, textHeight;
    TTF_SizeText(gFont, "click me!", &textWidth, &textHeight);

    cerr << textWidth << " " << textHeight << endl;

    /// Setting up the window. THERE SHOULD ONLY BE 1 WINDOW, DECLARED GLOBALLY
    gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
    ///

    vector<Button> buttons;
    buttons.emplace_back(Button("click me!", {0, 0, 300, 100}, white, 10, black, white, {255, 255, 255, 205}));

    buttons.emplace_back(Button("no, me!", {0, 150, 300, 100}, white, 10, black, white, {255, 255, 255, 205}));

    SDL_Event event;
    bool closed = 0;
    while (!closed) {
        while (SDL_PollEvent(&event)) {
            for (auto &i : buttons) i.HandleEvent(&event);
            switch (event.type) {
                default:
                    continue;
                case SDL_QUIT:
                    closed = 1;
                    break;
            }
        }

        for (auto &i : buttons) i.Display_Button();
        RenderFrame();
        SDL_Delay(1000 / 120);
    }

    Cleanup();
    SDL_Quit();

    return 0;
}
