#ifndef BUTTON_H
#define BUTTON_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "globals.h"
#include "layer.h"
using namespace std;

/// If you ever need a button/text box with a specific feature (aka do something if pressed/clicked) write your own OnClick function and pass it to the Button constructor. IMPORTANT: The function should return a boolean value of True if it has called the destructor and False otherwise.

class Button: public EventReceiver {
public:
     Button(const string& msg, const SDL_Rect& rect, const int& rimWidth, function<bool()> onClick, bool* scrFlashCheck = nullptr,
            bool* dependencies = nullptr, int WrapLength = 0)
            : text(msg), TextFont(gFont), rim(rect), rimColor(white), centerColor(black), textColor(white), flashColor(offwhite),
            FlashEndAction(onClick), wrapLength(WrapLength), dependencies(dependencies), ButtonFlashing(scrFlashCheck) {
        center = rim;
        center.x += rimWidth; center.y += rimWidth; center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;

        textSurface = TTF_RenderText_Solid_Wrapped(TextFont, text.c_str(), textColor, wrapLength);

        centerText.w = textSurface -> w, centerText.h = textSurface -> h;
        centerText.x = center.x + (center.w - centerText.w) / 2;
        centerText.y = center.y + (center.h - centerText.h) / 2;
    }

    void SetText(const string& msg) {
        text = msg;
    }

    void SetColor(const SDL_Color& RimColor, const SDL_Color& InnerColor, const SDL_Color& TextColor, const SDL_Color& FlashColor) {
        rimColor = RimColor; centerColor = InnerColor; textColor = TextColor; flashColor = FlashColor;
    }

    void SetFont(TTF_Font* const target) {
        TextFont = target;
    }

    bool HandleEvent(const SDL_Event* event) final {
        if (event -> type == SDL_MOUSEBUTTONDOWN && event -> button.button == SDL_BUTTON_LEFT && isHovered) {
            if (ButtonFlashing != nullptr && !*ButtonFlashing) {
                inFlash = *ButtonFlashing = 1;
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
                textSurface = TTF_RenderText_Solid_Wrapped(TextFont, text.c_str(), flashColor, wrapLength);
            else
                textSurface = TTF_RenderText_Solid_Wrapped(TextFont, text.c_str(), textColor, wrapLength);

            textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

            if (++flashFrameCounter == 60) {
                inFlash = 0;
                if (ButtonFlashing != nullptr) *ButtonFlashing = 0;
                if (FlashEndAction()) return;
            }
        }
        else {
            flashFrameCounter = 0;
            textSurface = TTF_RenderText_Solid_Wrapped(TextFont, text.c_str(), textColor, wrapLength);
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
    bool* dependencies = nullptr;
    int flashFrameCounter = 0, wrapLength = 0;
    function<bool()> FlashEndAction;

    TTF_Font* TextFont = nullptr;
};

class TextBox: public EventReceiver {
public:
    void RenderText() {
        if (textSurface) SDL_FreeSurface(textSurface);
        if (textTexture) SDL_DestroyTexture(textTexture);

        textSurface = TTF_RenderText_Solid_Wrapped(TextFont, text.c_str(), textColor, wrapLength);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    }

    TextBox(const string& msg, const SDL_Rect& rect, const int& rimWidth, bool* displayed = nullptr, bool* dependencies = nullptr,
            bool* scrFlashCheck = nullptr, int WrapLength = 0)
            : text(msg), TextFont(gFont), rim(rect), rimColor(white), centerColor(black), textColor(white), dependencies(dependencies),
            displayed(displayed), ButtonFlashing(scrFlashCheck), wrapLength(WrapLength) {
        center = rim;
        center.x += rimWidth; center.y += rimWidth;
        center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;

        RenderText();

        centerText.w = textSurface -> w, centerText.h = textSurface -> h;
        centerText.x = center.x + (center.w - centerText.w) / 2;
        centerText.y = center.y + (center.h - centerText.h) / 2;
    }

    ~TextBox() {
        if (textSurface) SDL_FreeSurface(textSurface);
        if (textTexture) SDL_DestroyTexture(textTexture);
    }

    void SetText(const string& msg) {
        text = msg;
        RenderText();
    }

    void SetColor(const SDL_Color& RimColor, const SDL_Color& InnerColor, const SDL_Color& TextColor) {
        rimColor = RimColor; centerColor = InnerColor; textColor = TextColor;
        RenderText();
    }

    void SetPosition(const SDL_Rect* pos = nullptr, int dx = -1, int dy = -1) {
        if (pos) {
            int rimWidth = center.x - rim.x;
            rim = *pos;
            center.x += rimWidth; center.y += rimWidth;
            center.h -= 2 * rimWidth; center.w -= 2 * rimWidth;
        }
        if (!~dx) centerText.x = center.x + (center.w - centerText.w) / 2;
        else centerText.x = center.x + dx;
        if (!~dy) centerText.y = center.y + (center.h - centerText.h) / 2;
        else centerText.y = center.y + dy;

        RenderText();
    }

    void SetFont(TTF_Font* const target) {
        TextFont = target;
    }

    bool HandleEvent(const SDL_Event* event) final {
        if (event -> type == SDL_MOUSEMOTION) {
            if (isHovered != inside(event -> motion.x, event -> motion.y)) {
                isHovered = !isHovered;
                if (dependencies) {
                    if (ButtonFlashing && *ButtonFlashing) *dependencies = 0;
                    else *dependencies = isHovered;
                }
                return isHovered;
            }
        }
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
    bool inside(int x, int y) {
        if (x < rim.x) return false;
        if (x > rim.x + rim.w) return false;
        if (y < rim.y) return false;
        if (y > rim.y + rim.h) return false;

        return true;
    }

    SDL_Rect rim, center, centerText;
    SDL_Color rimColor, centerColor, textColor;
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    TTF_Font* TextFont = nullptr;
    string text = "";
    bool *displayed = nullptr, *dependencies = nullptr, *ButtonFlashing = nullptr;
    bool isHovered = 0;
    int wrapLength = 0;
};

#endif // BUTTON_H
