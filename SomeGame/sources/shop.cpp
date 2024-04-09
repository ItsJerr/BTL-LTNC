#include "shop.h"

ShopLayer::ShopItem::ShopItem(const SDL_Rect& pos, const string& ItemDescription, int* ItemLevel, function<int(int)> ItemCost, SDL_Texture* CoinTexture, SDL_Texture* Logo): ItemLevel(ItemLevel), GetCost(ItemCost), CoinTexture(CoinTexture), Logo(Logo) {

}
