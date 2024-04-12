#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"
#include "globals.h"

class gamePlayLayer: public Layer {
public:
    gamePlayLayer();
    ~gamePlayLayer();

   // void HandleEvent(const SDL_Event* event) final;
    void Display() final;

private:
    vector<Stats*> Status;
    deque<Stats*> Messages;
};
class Player: public EventReceiver{
public:
    SDL_Rect rim, center;
    SDL_Color rimColor;
    int wrapLength = 0;
    string imagePath;
    SDL_Texture* texture = nullptr;
    Player(const string& imagePath, const SDL_Rect& rect, const int& rimWidth,
        int WrapLength = 0)
        : rim(rect), rimColor({255, 255, 255, 255}),
          wrapLength(WrapLength),
        imagePath(imagePath){
        LoadImage(imagePath);
        center = rim;
        center.x += rimWidth; center.y += rimWidth; center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;
    }
    ~Player(){
        SDL_DestroyTexture(texture);
    }
    bool HandleEvent(const SDL_Event* event) override{
        if(event -> type == SDL_KEYDOWN){
            if (event->key.keysym.sym == SDLK_DOWN) {
                center.y += speed;
                cerr << "DOWN\n";
                return true;
        }
        if (event->key.keysym.sym == SDLK_UP) {
                center.y -= speed;
                cerr << "UP\n";
                return true;
        }
            if (event->key.keysym.sym == SDLK_RIGHT) {
                center.x += speed;
                cerr << "RIGHT\n";
                return true;
        }


            if (event->key.keysym.sym == SDLK_LEFT) {
                center.x -= speed;
                cerr << "LEFT\n";
                return true;
        }
    }


        return false;
    }
    void DisplayAsset() override{
        SDL_RenderCopy(gRenderer, texture, nullptr, &center);
    }
    void LoadImage(const string& imagePath) {
        SDL_Surface* loadedSurface = IMG_Load(imagePath.c_str());
        if (!loadedSurface) {
            cerr << "Unable to load image " << imagePath << "! SDL_image Error: " << IMG_GetError() << endl;
            return;
        }
        texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (!texture) {
            cerr << "Unable to create texture from " << imagePath << "! SDL Error: " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
private:
};
#endif
