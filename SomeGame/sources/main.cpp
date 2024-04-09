//{ includes
#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"
#include "mainmenu.h"
#include "charactersaveload.h"
#include "charactermenu.h"
#include "savefile.h"
//}

using namespace std;

//{Globals. You hate 'em, but they are needed and cannot be avoided :/
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
TTF_Font* gFont = nullptr;
Engine* GameEngine = nullptr;

unsigned int FrameEventID;
unsigned int ChangeModeEventID;

int CURRENTMODE;

Layer* CurrentLayer;
//}

void ChangeMode(int ModeID, void* Data = nullptr) {
    delete CurrentLayer; CurrentLayer = nullptr;
    switch (ModeID) {
        default: {
            cerr << "Fatal error: Cannot find a mode with ID " << ModeID;
            exit(0);
        }
        case MAINMENUID: {
            CurrentLayer = new MainMenuLayer();
            break;
        }
        case NEWCHARACTERID: {
            CurrentLayer = new NewCharacterLayer();
            break;
        }
        case LOADCHARACTERID: {
            CurrentLayer = new LoadCharacterLayer();
            break;
        }
        case CHARACTERMENUID: {
            CurrentLayer = new CharacterMenuLayer();
            break;
        }
    }
    CURRENTMODE = ModeID;
}

void Init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    gFont = TTF_OpenFont("assets/fonts/dotty.ttf", 80);

    GameEngine = new Engine;

    /// Setting up the window & renderer. THERE SHOULD ONLY BE 1 WINDOW, DECLARED GLOBALLY
    gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    FrameEventID = SDL_RegisterEvents(2);
    ChangeModeEventID = FrameEventID + 1;

    /// Setting up main menu
    ChangeMode(NEWCHARACTERID);
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
                default: {
                    if (event.type == ChangeModeEventID) {
                        ChangeMode(*static_cast<int*>(event.user.data1), event.user.data2);
                        if (event.user.data1) delete static_cast<int*>(event.user.data1);
                        if (event.user.data2) delete static_cast<int*>(event.user.data1);
                    }
                    CurrentLayer -> HandleEvent(&event);
                    break;
                }
                case SDL_QUIT: {
                    closed = 1;
                    break;
                }
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
