#pragma once
#include<bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
TTF_Font* gFont = nullptr;

SDL_Color black = {0, 0, 0, 255}, white = {255, 255, 255, 255};

int CURRENT_MODE = 1000;

bool HandlingEvent = 0;

const int MAIN_MENU = 1000;
const int TUTORIAL_SCREEN = 2000;
const int IN_GAME = 3000;
const int SCREEN_MODE_SELECTION = 4000;


