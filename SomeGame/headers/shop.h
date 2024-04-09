#ifndef SHOP_H
#define SHOP_H

#include "engine.h"
#include "layer.h"
#include "button.h"

class ShopLayer: public Layer {
public:
    ShopLayer(int PrevModeID);
    ~ShopLayer();

    void Display() final;
private:
    // Items in the shop
    class ShopItem: public EventReceiver {
    public:
        ShopItem(const SDL_Rect& pos, const string& ItemDescription, function<void()> ItemUpgrade, function<int(int)> ItemCost, SDL_Texture* CoinTexture, SDL_Texture* Logo);
        ~ShopItem();

        bool HandleEvent(const SDL_Event* event) final;
        void DisplayAsset() final;
    private:
        // The buy button for the item. Basically just a normal button plus a coin symbol
        class ShopBuyButton : public Button {
        public:
            ShopBuyButton(const string& msg, const SDL_Rect& rect, const int& rimWidth, function<bool()> onClick,
                          SDL_Texture* CoinTexture, bool* scrFlashCheck = nullptr)
                : Button(msg, rect, rimWidth, onClick, scrFlashCheck), CoinTexture(CoinTexture) {}

            void DisplayAsset() final {
                Button::DisplayAsset();

                SDL_Rect coinRect = {centerText.x + centerText.w + 10, centerText.y, centerText.h, centerText.h};
                SDL_RenderCopy(gRenderer, CoinTexture, nullptr, &coinRect);
            }

        private:
            SDL_Texture* CoinTexture = nullptr;
        };

        function<int(int)> GetCost = nullptr;
        function<void()> Upgrade = nullptr;
        SDL_Texture *CoinTexture = nullptr, *Logo = nullptr;
        ShopBuyButton* BuyButton = nullptr;
        TextBox* Outer = nullptr;
    };

    SDL_Texture* CoinTexture = nullptr;
    vector<EventReceiver*> insiders;
};

#endif // SHOP_H
