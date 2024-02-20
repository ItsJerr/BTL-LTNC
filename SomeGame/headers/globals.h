#pragma once
#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;
TTF_Font* gFont = nullptr;

SDL_Color black = {0, 0, 0, 255}, white = {200, 200, 200, 76};
