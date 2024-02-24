#pragma once

#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "layer.h"
using namespace std;

/// really important lol
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
TTF_Font* gFont = nullptr;

/// some common colors
SDL_Color black = {0, 0, 0, 255}, white = {255, 255, 255, 255};

/// window resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

/// ID for the custom event signalling a frame
unsigned int FrameEventID;

/// Variables related to layers
Layer* CurrentLayer;

int CURRENTMODE;

const int MAINMENUID = 1000;
const int NEWGAMEID = 2000;
const int LOADGAMEID = 3000;
