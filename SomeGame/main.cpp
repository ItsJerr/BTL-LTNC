#include "headers/game.h"
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

    gFont = TTF_OpenFont("assets/fonts/joystix.ttf", 35);

    int textWidth, textHeight;
    TTF_SizeText(gFont, "click me!", &textWidth, &textHeight);

    cerr << textWidth << " " << textHeight << endl;

    /// Setting up the window. THERE SHOULD ONLY BE 1 WINDOW, DECLARED GLOBALLY
    gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
    ///

    Layer MainMenu = Layer(MAIN_MENU);
    Button NewGame = Button("New Game", {490, 282, 300, 75}, white, 3, black, white, {255, 255, 255, 125});

    Button LoadGame = Button("Load Game", {490, 363, 300, 75}, white, 3, black, white, {255, 255, 255, 125});

    MainMenu.addAsset(&NewGame); MainMenu.addAsset(&LoadGame);

    /// ONLY WORK THROUGH LAYERS, DO NOT DIRECTLY CALL ASSETS IN LAYERS FOR ANY REASONs
    vector<Layer*> Layers = {&MainMenu};

    SDL_Event event;
    bool closed = 0;
    while (!closed) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                default:
                    for (const auto UI: Layers) if (UI -> HandleEvent(&event))
                        continue;
                    continue;
                case SDL_QUIT:
                    closed = 1;
                    break;
            }
        }

        for (const auto UI: Layers) UI -> Display();
        RenderFrame();
        SDL_Delay(1000 / 120);
    }

    Cleanup();
    SDL_Quit();

    return 0;
}
