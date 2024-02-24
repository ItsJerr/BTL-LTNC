#ifndef GLOBALS_H
#define GLOBALS_H

#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using namespace std;

/// really important lol
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_Texture* gTexture;
extern TTF_Font* gFont;

/// some common colors
#define black {0, 0, 0, 255}
#define white {255, 255, 255, 255}

/// window resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

/// ID for the custom event signalling a frame
extern unsigned int FrameEventID;

extern int CURRENTMODE;

const int MAINMENUID = 1000;
const int NEWGAMEID = 2000;
const int LOADGAMEID = 3000;
#endif // GLOBALS_H
