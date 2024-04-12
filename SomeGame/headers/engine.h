#ifndef ENGINE_H
#define ENGINE_H

#include<bits/stdc++.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "gamevar.h"

class Engine {
public:
    Engine() {
        gGameData = new GameData;
    }

    ~Engine() {
        delete gGameData;
    }

    GameData* gGameData;
};
class Stats{
    SDL_Rect rim, center, centerText;
    SDL_Color textColor;
    SDL_Surface* textSurface = NULL;
    SDL_Texture* textTexture = NULL;
    string text = "";
    int wrapLength = 0;
public:
    Stats(const string& msg, const SDL_Rect& rect, int WrapLength);
    ~Stats() {
        if (textSurface) SDL_FreeSurface(textSurface);
        if (textTexture) SDL_DestroyTexture(textTexture);
    }
    void SetText(const string& msg);
    void SetColor(const SDL_Color& TextColor);
    void SetPosition(const SDL_Rect* pos, int dx, int dy);
    void DisplayAsset();
};
class Icon {
    public:
    Icon(const string &SpriteSheet, SDL_Rect Box, int WrapLength = 0)
    :rim(rect), wrapLength(WrapLength), imagePath(SpriteSheet){
    
    

    center = rim;

    }
    ~Icon(){
        if(textSurface) SDL_FreeSurface;
        if(textTexture) SDL_DestroyTexture(textTexture);
    }
    void DisplayAsset();

}
#endif // ENGINE_H
