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
extern TTF_Font* gFont;
extern TTF_Font* gameFont;
extern SDL_Texture* TileSet;

class Engine;
extern Engine* gEngine;

/// some common colors
const SDL_Color black = SDL_Color{0, 0, 0, 255};
const SDL_Color white = SDL_Color{255, 255, 255, 230};
const SDL_Color offwhite = SDL_Color{255, 255, 255, 100};
const SDL_Color transparent = SDL_Color{0, 0, 0, 0};

/// window resolution
const int SCREEN_WIDTH = 1560;
const int SCREEN_HEIGHT = 900;

/// ID for the custom event signaling a frame
extern unsigned int FrameEventID;
/// ID for the custom event signaling changing mode
extern unsigned int ChangeModeEventID;

extern int CURRENTMODE;

const int MAINMENUID = 1000;
const int NEWCHARACTERID = 2000;
const int LOADCHARACTERID = 3000;
const int CHARACTERMENUID = 4000;
const int INGAMEID = 5000;
const int SHOPID = 6000;

int rnd(int l, int r);
int rround(float x);
float frnd(float a, float b);

#endif // GLOBALS_H
