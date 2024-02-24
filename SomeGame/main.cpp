//{ includes
#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "headers/globals.h"
#include "headers/layer.h"
#include "headers/button.h"
#include "headers/mainmenu.h"
//}

using namespace std;

/// INSERT LAYERS IN THE ORDER YOU WANT TO HANDLE EVENTS

void Init() {
    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING) != 0;
    TTF_Init() != 0;

    gFont = TTF_OpenFont("assets/fonts/dotty.ttf", 80);

    /// Setting up the window & renderer. THERE SHOULD ONLY BE 1 WINDOW, DECLARED GLOBALLY
    gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    FrameEventID = SDL_RegisterEvents(1);

    /// Setting up main menu
    CURRENTMODE = MAINMENUID;
    CurrentLayer = new MainMenuClass(MAINMENUID);
}

/// call once per frame. clears the renderer so needs to redraw everything
void RenderFrame() {
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
}

void Cleanup() { /// call before everything
    SDL_DestroyTexture(gTexture); gTexture = nullptr;
    SDL_DestroyRenderer(gRenderer); gRenderer = nullptr;
    SDL_DestroyWindow(gWindow); gWindow = nullptr;
}

signed main(int argc, char *argv[]) {
    Init();

    SDL_Event event;
    bool closed = 0;
    while (!closed) {
        /// Setup FrameEvent once per frame so that EventReceivers know when a frame has passed.
        SDL_Event FrameEvent; SDL_zero(FrameEvent);
        FrameEvent.type = FrameEventID;
        SDL_PushEvent(&FrameEvent);

        /// Event loop, try not to modify
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                default:
                    CurrentLayer -> HandleEvent(&event);
                    break;
                case SDL_QUIT:
                    closed = 1;
                    break;
            }
        }

        /// Render loop
        CurrentLayer -> Display();
        RenderFrame();

        /// 60 FPS. TODO: Implement this better using some kind of clock/tick counter
        SDL_Delay(1000 / 60);
    }

    Cleanup();
    SDL_Quit();

    return 0;
}
