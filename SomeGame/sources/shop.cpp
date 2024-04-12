#include "shop.h"

ShopItem::ShopItem(const SDL_Rect& pos, const string& ItemDescription, function<bool()> ItemUpgrade, function<int()> ItemCost, const string& LogoPath, SDL_Texture* CoinTexture)
                : GetCost(ItemCost), Upgrade(ItemUpgrade), CoinTexture(CoinTexture) {
    SDL_Surface* LogoSurface = IMG_Load(LogoPath.c_str());
    LogoTexture = SDL_CreateTextureFromSurface(gRenderer, LogoSurface);
    SDL_FreeSurface(LogoSurface);

    LogoPosition = {pos.x + 5, pos.y + 5, pos.h - 10, pos.h - 10};

    // for some reason, TTF_RenderText_Solid_Wrapped does not allow for texts with zero width, so the space is here to circumvent that
    Outer = new TextBox(" ", pos, 3, nullptr, &Hovered, nullptr);
    Description = new TextBox(ItemDescription, {-10, 830, SCREEN_WIDTH + 10, 75}, 3, &Hovered);

    ShopPurchaseButton = new PurchaseButton(to_string(ItemCost()), {pos.x + pos.w - 215, pos.y + (pos.h - 100) / 2, 200, 100}, 3, ItemUpgrade, CoinTexture, nullptr);
}

ShopItem::~ShopItem() {
    SDL_DestroyTexture(LogoTexture);
    delete Outer;
    delete ShopPurchaseButton;
}

bool ShopItem::HandleEvent(const SDL_Event* event) {
    Outer -> HandleEvent(event);
    ShopPurchaseButton -> HandleEvent(event);
    Description -> HandleEvent(event);
}

void ShopItem::DisplayAsset() {
    Outer -> DisplayAsset();
    ShopPurchaseButton -> DisplayAsset();
    Description -> DisplayAsset();
    SDL_RenderCopy(gRenderer, LogoTexture, nullptr, &LogoPosition);
}

ShopLayer::ShopLayer(int PrevModeID) {
    insiders.push_back(new Particles(offwhite));
    insiders.push_back(new Button("back", {20, 20, 120, 50}, 3, [PrevModeID]() -> bool {
        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(PrevModeID);
        SDL_PushEvent(&tmp);
        return 0;
    }, &ButtonFlashCheck));

    SDL_Surface* CoinSurface = IMG_Load("assets/img/coin.png");
    CoinTexture = SDL_CreateTextureFromSurface(gRenderer, CoinSurface);
    SDL_FreeSurface(CoinSurface);

    insiders.push_back(new ShopTextBox(to_string(gEngine -> gGameData -> CoinBalance), {1350, 20, 210, 50}, 0, CoinTexture, &ButtonFlashCheck));
    insiders.push_back(new ShopItem({40, 90, 720, 160}, "Increase armor effectiveness", bind(&Engine::HPUpgrade, gEngine), bind(&Engine::LuckUpgradeCost, gEngine), "assets/img/potion_red.png", CoinTexture));
    insiders.push_back(new ShopItem({800, 90, 720, 150}, "Testing abc def ghi jkl mno pqr stu vwx yz", bind(&Engine::HPUpgrade, gEngine), bind(&Engine::HPUpgradeCost, gEngine), "assets/img/potion_blue.png", CoinTexture));
}

ShopLayer::~ShopLayer() {
    for (EventReceiver* asset : insiders) delete asset;
    insiders.clear();
    SDL_DestroyTexture(CoinTexture);
}

void ShopLayer::Display() {
    for (EventReceiver* asset : insiders) asset -> DisplayAsset();
}

void ShopLayer::HandleEvent(const SDL_Event* event) {
    for (EventReceiver* asset : insiders) asset -> HandleEvent(event);
}
