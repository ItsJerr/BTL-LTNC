#include "engine.h"
#include "globals.h"

Stats::Stats(const string& msg, const SDL_Rect& rect, int WrapLength = 0)
            : text(msg), rim(rect), wrapLength(WrapLength), textColor(white) {
        center = rim;

        TTF_SizeText(smallGFont, text.c_str(), &centerText.w, &centerText.h);
        centerText.x = center.x + (center.w - centerText.w) / 2;
        centerText.y = center.y;
        textSurface = TTF_RenderText_Solid_Wrapped(smallGFont, text.c_str(), textColor, wrapLength);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
}
void Stats::SetText(const string& msg){
	text = msg;

    if (textSurface) SDL_FreeSurface(textSurface);
    if (textTexture) SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid_Wrapped(gFont, text.c_str(), textColor, wrapLength);
    textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
}

void Stats::SetColor(const SDL_Color& TextColor) {
    textColor = TextColor;

    if (textSurface) SDL_FreeSurface(textSurface);
    if (textTexture) SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid_Wrapped(gFont, text.c_str(), textColor, wrapLength);
    textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
}

void Stats::SetPosition(const SDL_Rect* pos = nullptr, int dx = -1, int dy = -1) {
    if (pos) {
        rim = *pos;
    }
    if (!~dx && !~dy) {
        centerText.x = rim.x;
        centerText.y = center.y + (center.h - centerText.h) / 2;
    }
    else {
        centerText.x = center.x + dx;
        centerText.y = center.y + dy;
    }

    if (textSurface) SDL_FreeSurface(textSurface);
    if (textTexture) SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid_Wrapped(gFont, text.c_str(), textColor, wrapLength);
    textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
}


void Stats::DisplayAsset() {
    SDL_RenderCopy(gRenderer, textTexture, nullptr, &centerText);
}
