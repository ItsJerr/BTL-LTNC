#pragma once
#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
using namespace std;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
TTF_Font* gFont = nullptr;

SDL_Color black = {0, 0, 0, 255}, white = {255, 255, 255, 255};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

unsigned int FrameEventID;

int CURRENT_MODE;

bool ButtonFlashing = 0;

const int MAIN_MENU = 1000;
const int TUTORIAL_SCREEN = 2000;
const int IN_GAME = 3000;
const int SCREEN_MODE_SELECTION = 4000;
