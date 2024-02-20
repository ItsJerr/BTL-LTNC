#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"
using namespace std;

class Button {
public:
    Button(string msg, SDL_Rect _rim, SDL_Color outer, SDL_Rect _center, SDL_Color inner, SDL_Color _textColor) {
        text = msg;
        rim = _rim;
        center = _center;
        rimColor = outer;
        centerColor = inner;
        textColor = _textColor;

        textSurface = TTF_RenderText_Solid(gFont, text.c_str(), white);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    }

    void Display_Button() {
        SDL_SetRenderDrawColor(gRenderer, rimColor.r, rimColor.g, rimColor.b, rimColor.a);
        SDL_RenderFillRect(gRenderer, &rim);

        SDL_SetRenderDrawColor(gRenderer, centerColor.r, centerColor.g, centerColor.b, centerColor.a);
        SDL_RenderFillRect(gRenderer, &center);

        SDL_RenderCopy(gRenderer, textTexture, nullptr, &center);
    }
private:
    SDL_Rect rim, center;
    SDL_Color rimColor, centerColor, textColor;
    SDL_Surface* textSurface; SDL_Texture* textTexture;
    string text = "";
    int description = 0;
};


