#ifndef SHOP_H
#define SHOP_H

#include "engine.h"
#include "layer.h"
#include "button.h"

// The buy button for the item. Basically just a normal button plus a coin symbol
class PurchaseButton : public Button {
public:
    PurchaseButton(const string& msg, const SDL_Rect& rect, const int& rimWidth, function<bool()> onClick,
                  SDL_Texture* CoinTexture, bool* scrFlashCheck = nullptr)
        : Button(msg, rect, rimWidth, onClick, scrFlashCheck), CoinTexture(CoinTexture) {
            // The text should be right-aligned
            centerText.x = center.x + center.w - (centerText.w + centerText.h);
            centerText.y = center.y + (center.h - centerText.h) / 2;
        }

    void DisplayAsset() final {
        Button::DisplayAsset();

        SDL_Rect coinRect = {centerText.x + centerText.w, centerText.y, centerText.h, centerText.h};
        SDL_RenderCopy(gRenderer, CoinTexture, nullptr, &coinRect);
    }
private:
    SDL_Texture* CoinTexture = nullptr;
};

// A textbox class for the shop. Basically just a normal TextBox plus a coin symbol, same as above
class ShopTextBox : public TextBox {
public:
    ShopTextBox(const string& msg, const SDL_Rect& rect, const int& rimWidth,
                  SDL_Texture* CoinTexture, bool* scrFlashCheck = nullptr)
        : TextBox(msg, rect, rimWidth, nullptr, nullptr, scrFlashCheck), CoinTexture(CoinTexture) {
            // The text should be right-aligned
            centerText.x = center.x + center.w - (centerText.w + centerText.h);
            centerText.y = center.y + (center.h - centerText.h) / 2;
        }

    void DisplayAsset() final {
        TextBox::DisplayAsset();

        SDL_Rect coinRect = {centerText.x + centerText.w, centerText.y, centerText.h, centerText.h};
        SDL_RenderCopy(gRenderer, CoinTexture, nullptr, &coinRect);
    }
private:
    SDL_Texture* CoinTexture = nullptr;
};

// Items in the shop
class ShopItem: public EventReceiver {
public:
    ShopItem(const SDL_Rect& pos, const string& ItemDescription, function<bool()> ItemUpgrade, function<int()> ItemCost, const string& LogoPath, SDL_Texture* CoinTexture);
    ~ShopItem();

    bool HandleEvent(const SDL_Event* event) final;
    void DisplayAsset() final;
private:
    function<int()> GetCost = nullptr;
    function<bool()> Upgrade = nullptr;
    SDL_Texture *CoinTexture = nullptr, *LogoTexture = nullptr;
    EventReceiver* ShopPurchaseButton = nullptr;
    TextBox *Outer = nullptr, *Description = nullptr;
    SDL_Rect LogoPosition;
    bool Hovered;
};

class ShopLayer: public Layer {
public:
    ShopLayer(int PrevModeID);
    ~ShopLayer();

    void Display() final;
    void HandleEvent(const SDL_Event* event) final;
private:
    SDL_Texture* CoinTexture = nullptr;
    vector<EventReceiver*> insiders;
    bool ButtonFlashCheck = 0;
};

#endif // SHOP_H
