#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"
using namespace std;

/*
a clickable button that can detect whether it is hovered on/clicked to call other stuff. WIP
Notes:
- rim is made by stacking 2 rectangles, 1 containing the rim and 1 containing the inside color.
- text is NOT STRETCHED, instead made to center inside the inner rectangle
*/

class Button: public EventReceiver {
public:
    Button(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor) {
        text = msg;

        rim = _rim;
        center = rim; center.x += rim_width; center.y += rim_width; center.h -= 2 * rim_width; center.w -= 2 * rim_width;

        rimColor = outer; centerColor = inner; textColor = _textColor; flashColor = _flashColor;

        TTF_SizeText(gFont, text.c_str(), &centerText.w, &centerText.h);
        centerText.x = center.x + (center.w - centerText.w) / 2;
        centerText.y = center.y + (center.h - centerText.h) / 2;
    }

    bool HandleEvent(const SDL_Event* event) override {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered) {
            if (ButtonFlashing) return 0;
            inFlash = ButtonFlashing = 1;
            return 1;
        }
        else if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                return isHovered;
            }
        }
        return false;
    }

    void DisplayAsset() override {
        if (textSurface) {
            SDL_FreeSurface(textSurface);
            textSurface = nullptr;
        }
        if (textTexture) {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }
        if (inFlash == 1) {
            /// 3 ticks per sec, ticks for 1 sec

            if ((flashFrameCounter / 10) % 2 == 0)
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), flashColor);
            else
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);

            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            if (++flashFrameCounter == 60) {
                inFlash = 0;
                ButtonFlashing = 0;
            }
        }
        else {
            flashFrameCounter = 0;
            textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        }

        /// draw outer rectangle
        SDL_SetRenderDrawColor(gRenderer, rimColor.r, rimColor.g, rimColor.b, rimColor.a);
        SDL_RenderFillRect(gRenderer, &rim);

        /// draw inner rectangle
        SDL_SetRenderDrawColor(gRenderer, centerColor.r, centerColor.g, centerColor.b, centerColor.a);
        SDL_RenderFillRect(gRenderer, &center);

        /// print message
        SDL_RenderCopy(gRenderer, textTexture, nullptr, &centerText);
    }
private:
    bool inside(int x, int y) {
        if (x < rim.x) return false;
        if (x > rim.x + rim.w) return false;
        if (y < rim.y) return false;
        if (y > rim.y + rim.h) return false;

        return true;
    }

    SDL_Rect rim, center, centerText;
    SDL_Color rimColor, centerColor, textColor, flashColor;
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    string text = "";

    bool inFlash = 0, isHovered = 0;
    int flashFrameCounter = 0;
};


