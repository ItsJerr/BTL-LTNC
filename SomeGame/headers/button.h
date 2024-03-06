#ifndef BUTTON_H
#define BUTTON_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"
#include "layer.h"
using namespace std;

/// If you ever need a button/text box with a specific feature (aka do something if pressed/clicked) write your own function and pass it to the Button constructor.

class Button: public EventReceiver {
public:
     Button(const string& msg, const SDL_Rect& rect, const SDL_Color& outerColor, const int& rimWidth,
            const SDL_Color& innerColor, const SDL_Color& textColor, const SDL_Color& flashColor,
            function<void()> onClick, bool* scrFlashCheck = nullptr,
            bool* dependencies = nullptr, bool clickable = false)
            : text(msg), rim(rect), rimColor(outerColor), centerColor(innerColor),
              textColor(textColor), flashColor(flashColor), FlashEndAction(onClick),
              dependencies(dependencies), ButtonFlashing(scrFlashCheck), clickable(clickable) {
        center = rim;
        center.x += rimWidth; center.y += rimWidth; center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;

        TTF_SizeText(gFont, text.c_str(), &centerText.w, &centerText.h);
        centerText.x = center.x + (center.w - centerText.w) / 2 + rimWidth;
        centerText.y = center.y + (center.h - centerText.h) / 2 + rimWidth;

        textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    }

    virtual void ModifyText(string msg) {
        text = msg;
    }

    bool HandleEvent(const SDL_Event* event) override {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered && clickable) {
            if (ButtonFlashing != nullptr && !*ButtonFlashing) {
                inFlash = 1;
                if (ButtonFlashing != nullptr) *ButtonFlashing = 1;
                return 1;
            }
        }
        else if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                if (inFlash && dependencies) *dependencies = 1;
                else if (dependencies) {
                    if (ButtonFlashing && *ButtonFlashing && !inFlash) *dependencies = 0;
                    else *dependencies = isHovered;
                }
            }
        }
        return false;
    }

    ~Button() {
        if (textSurface) SDL_FreeSurface(textSurface);
        if (textTexture) SDL_DestroyTexture(textTexture);
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

    bool inFlash = 0, isHovered = 0, clickable = 0;
    bool* ButtonFlashing = nullptr;
    bool* dependencies = nullptr;
    int flashFrameCounter = 0;
    function<void()> FlashEndAction;
};

class TextBox: public EventReceiver {
public:
    TextBox(const string& msg, const SDL_Rect& rect, const SDL_Color& outerColor, const int& rimWidth,
            const SDL_Color& innerColor, const SDL_Color& textColor,
            bool* displayed = nullptr, bool* dependencies = nullptr, bool centered = true)
            : text(msg), rim(rect), rimColor(outerColor), centerColor(innerColor),
              textColor(textColor), dependencies(dependencies), displayed(displayed) {
        center = rim;
        center.x += rimWidth; center.y += rimWidth;
        center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;

        TTF_SizeText(gFont, text.c_str(), &centerText.w, &centerText.h);

        if (centered) {
            centerText.x = center.x + (center.w - centerText.w) / 2;
            centerText.y = center.y + (center.h - centerText.h) / 2;
        }
        else {
            centerText.x = center.x + 5;
            centerText.y = center.y + 5;
        }

        textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    }

    TextBox(const string& msg, const SDL_Rect& rect, const SDL_Color& outerColor, const int& rimWidth,
            const SDL_Color& innerColor, const SDL_Color& textColor, const pair<int, int>& position,
            bool* displayed = nullptr, bool* dependencies = nullptr)
            : text(msg), rim(rect), rimColor(outerColor), centerColor(innerColor),
              textColor(textColor), dependencies(dependencies), displayed(displayed) {
        center = rim;
        center.x += rimWidth; center.y += rimWidth;
        center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;

        TTF_SizeText(gFont, text.c_str(), &centerText.w, &centerText.h);
        tie(centerText.x, centerText.y) = position;

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
        if (displayed != nullptr && !*displayed) return;
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
    bool *displayed = nullptr, *dependencies = nullptr;
};

#endif // BUTTON_H
