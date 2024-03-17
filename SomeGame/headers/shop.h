#ifndef SHOP_H
#define SHOP_H

#include "engine.h"
#include "layer.h"

class ShopLayer: public Layer {
public:
    ShopLayer(int PrevModeID);
    ~ShopLayer();
private:
    class ShopItem: public EventReceiver {
    public:
        ShopItem(const string& ItemName, const string& ItemDescription, int* ItemLevel, function<int(int)> ItemCost);
        ~ShopItem();

        bool HandleEvent(const SDL_Event* event) final;
        void DisplayAsset() final;
    private:
        string name = "", description = "";
        int* level = nullptr;
        function<int(int)> GetCost = nullptr;
    };
};

#endif // SHOP_H
