#ifndef BUTTON_H
#define BUTTON_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"
#include "layer.h"
using namespace std;

/// If you ever need a button/text box with a specific feature (aka do something if pressed/clicked) please write your own class and override the function FlashEndAction().

class Button: public EventReceiver {
public:
    Button(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, SDL_Color _flashColor, bool* ScrFlashChk = nullptr, bool* DispChk = nullptr): text(msg), rim(_rim), rimColor(outer), centerColor(inner), textColor(_textColor), flashColor(_flashColor), DisplayCheck(DispChk), ButtonFlashing(ScrFlashChk) {
        center = rim;
        center.x += rim_width; center.y += rim_width; center.h -= 2 * rim_width; center.w -= 2 * rim_width;

        TTF_SizeText(gFont, text.c_str(), &centerText.w, &centerText.h);
        centerText.x = center.x + (center.w - centerText.w) / 2;
        centerText.y = center.y + (center.h - centerText.h) / 2;

        textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    }

    virtual void ModifyText(string msg) {
        text = msg;
    }

    bool HandleEvent(const SDL_Event* event) override {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered) {
            if (ButtonFlashing != nullptr && !*ButtonFlashing) {
                inFlash = 1;
                if (ButtonFlashing != nullptr) *ButtonFlashing = 1;
                return 1;
            }
        }
        else if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                if (inFlash && DisplayCheck) *DisplayCheck = 1;
                else if (DisplayCheck) {
                    if (ButtonFlashing && *ButtonFlashing && !inFlash) *DisplayCheck = 0;
                    else *DisplayCheck = isHovered;
                }
                return isHovered;
            }
        }
        return false;
    }

    virtual ~Button() {
        if (textSurface) SDL_FreeSurface(textSurface);
        if (textTexture) SDL_DestroyTexture(textTexture);
    }

    virtual void FlashEndAction() {}

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
            /// 3 ticks per sec, flashes for 1 sec
            if ((flashFrameCounter / 10) % 2 == 0)
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), flashColor);
            else
                textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);

            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            if (++flashFrameCounter == 60) {
                inFlash = 0;
                if (ButtonFlashing != nullptr) *ButtonFlashing = 0;
                FlashEndAction();
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
protected:
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
    bool* ButtonFlashing = nullptr;
    bool* DisplayCheck = nullptr;
    int flashFrameCounter = 0;
};

class TextBox: public EventReceiver {
public:
    TextBox(string msg, SDL_Rect _rim, SDL_Color outer, int rim_width, SDL_Color inner, SDL_Color _textColor, bool* _displayed) {
        text = msg;

        rim = _rim;
        center = rim; center.x += rim_width; center.y += rim_width; center.h -= 2 * rim_width; center.w -= 2 * rim_width;

        rimColor = outer; centerColor = inner; textColor = _textColor;

        TTF_SizeText(gFont, text.c_str(), &centerText.w, &centerText.h);
        centerText.x = center.x + (center.w - centerText.w) / 2;
        centerText.y = center.y + (center.h - centerText.h) / 2;

        displayed = _displayed;

        textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    }

    virtual ~TextBox() {
        if (textSurface) SDL_FreeSurface(textSurface);
        if (textTexture) SDL_DestroyTexture(textTexture);
    }

    bool HandleEvent(const SDL_Event* event) override {
        return false;
    }

    void DisplayAsset() override {
        if (displayed == nullptr || !*displayed) return;
        /// draw outer rectangle
        SDL_SetRenderDrawColor(gRenderer, rimColor.r, rimColor.g, rimColor.b, rimColor.a);
        SDL_RenderFillRect(gRenderer, &rim);

        /// draw inner rectangle
        SDL_SetRenderDrawColor(gRenderer, centerColor.r, centerColor.g, centerColor.b, centerColor.a);
        SDL_RenderFillRect(gRenderer, &center);

        /// print message
        SDL_RenderCopy(gRenderer, textTexture, nullptr, &centerText);
    }
protected:
    SDL_Rect rim, center, centerText;
    SDL_Color rimColor, centerColor, textColor;
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    string text = "";
    bool* displayed = nullptr;
};
#endif // BUTTON_H
