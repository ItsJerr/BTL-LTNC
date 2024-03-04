#ifndef GLOBALS_H
#define GLOBALS_H

#include <bits/stdc++.h>
#include "gamevar.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using namespace std;

/// really important lol
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_Texture* gTexture;
extern TTF_Font* gFont;

extern GameData* gGameData;

/// some common colors
const SDL_Color black = SDL_Color{0, 0, 0, 255};
const SDL_Color white = SDL_Color{255, 255, 255, 255};
const SDL_Color offwhite = SDL_Color{255, 255, 255, 100};
const SDL_Color transparent = SDL_Color{0, 0, 0, 0};

/// window resolution
const int SCREEN_WIDTH = 1560;
const int SCREEN_HEIGHT = 900;

/// ID for the custom event signalling a frame
extern unsigned int FrameEventID;
/// ID for the custom event signalling changing mode
extern unsigned int ChangeModeEventID;

extern int CURRENTMODE;

const int MAINMENUID = 1000;
const int NEWCHARACTERID = 2000;
const int LOADCHARACTERID = 3000;
const int CHARACTERMAINMENUID = 4000;

#endif // GLOBALS_H
