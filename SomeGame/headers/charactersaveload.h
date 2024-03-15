#ifndef LOADCHARACTER_H
#define LOADCHARACTER_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"
#include "savefile.h"

class LoadCharacterLayer: public Layer {
public:
    LoadCharacterLayer();
    ~LoadCharacterLayer();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    vector<EventReceiver*> insiders;

    SDL_Texture* SaveFilePreviews[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    bool DisplayingPreview[5] = {0, 0, 0, 0, 0}, ButtonFlashing = 0;
};

class NewCharacterLayer: public Layer {
public:
    NewCharacterLayer();
    ~NewCharacterLayer();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    class ConfirmationBox: public EventReceiver {
    public:
        ConfirmationBox(const int& idx, NewCharacterLayer& parent);
        ~ConfirmationBox();

        bool HandleEvent(const SDL_Event* event) final;
        void DisplayAsset() final;

    private:
        function<bool()> YesClick, NoClick;

        Button *YesButton = nullptr, *NoButton = nullptr;
        TextBox* Prompt = nullptr;
        bool ButtonFlashing = 0;
    };

    vector<EventReceiver*> insiders;

    SDL_Texture* SaveFilePreviews[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    bool DisplayingPreview[5] = {0, 0, 0, 0, 0}, ButtonFlashing = 0;
};

#endif // NEWGAME_H
