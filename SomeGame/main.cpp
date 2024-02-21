#include "headers/game.h"
using namespace std;

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

/// INSERT LAYERS IN THE ORDER YOU WANT TO HANDLE EVENTS
vector<Layer*> Layers;

void Init() {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError("SDL_Init", 1);
    if (TTF_Init() != 0) logTTFError("TTF_Init", 1);

    gFont = TTF_OpenFont("assets/fonts/joystix.ttf", 35);

    /// Setting up the window & renderer. THERE SHOULD ONLY BE 1 WINDOW, DECLARED GLOBALLY
    gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    FrameEventID = SDL_RegisterEvents(1);

    /// Setting up main menu
    MainMenuNamespace::init(); Layers.push_back(&MainMenuNamespace::MainMenu);
}

/// call once per frame. clears the renderer so needs to redraw everything
void RenderFrame() {
    SDL_RenderPresent(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(gRenderer);
}

void Cleanup() { /// call before everything
    SDL_DestroyTexture(gTexture); gTexture = nullptr;
    SDL_DestroyRenderer(gRenderer); gRenderer = nullptr;
    SDL_DestroyWindow(gWindow); gWindow = nullptr;

    Log.close();
}

signed main(int argc, char *argv[]) {
    Init();
    /// ONLY WORK THROUGH LAYERS, DO NOT DIRECTLY CALL ASSETS IN LAYERS FOR ANY REASONS

    SDL_Event event;
    bool closed = 0;
    while (!closed) {
        /// Setup FrameEvent once per frame so that EventReceivers know when a frame has passed.
        SDL_Event FrameEvent; SDL_zero(FrameEvent);
        FrameEvent.type = FrameEventID;
        SDL_PushEvent(&FrameEvent);

        /// Event loop
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                default:
                    for (const auto &UI: Layers) if (UI -> HandleEvent(&event))
                        continue;
                    continue;
                case SDL_QUIT:
                    closed = 1;
                    break;
            }
        }

        /// Render loop
        for (const auto &UI: Layers) UI -> Display();
        RenderFrame();

        /// 60 FPS. TODO: Implement this better using some kind of clock/tick counter
        SDL_Delay(1000 / 60);
    }

    Cleanup();
    SDL_Quit();

    return 0;
}
