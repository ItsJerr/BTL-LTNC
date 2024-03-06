#ifndef LOADCHARACTER_H
#define LOADCHARACTER_H

#include<bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "button.h"
#include "savefile.h"

class LoadCharacterClass: public Layer {
public:
    LoadCharacterClass();
    ~LoadCharacterClass();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    vector<EventReceiver*> insiders;

    Particles* FlashingParticles = nullptr;
    Button* SaveGameButtons[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    Button* BackButton = nullptr;
    SDL_Texture* SaveFilePreviews[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    bool DisplayingPreview[5] = {0, 0, 0, 0, 0}, SaveFileExistance[5] = {0, 0, 0, 0, 0}, ButtonFlashing = 0;

    TextBox *title = nullptr, *preview = nullptr;
};

class NewCharacterClass: public Layer {
public:
    NewCharacterClass();
    ~NewCharacterClass();

    void HandleEvent(const SDL_Event* event) final;
    void Display() final;
private:
    class ConfirmationBox: public EventReceiver {
    public:
        ConfirmationBox(const int& idx, NewCharacterClass& parent);
        ~ConfirmationBox();

        bool HandleEvent(const SDL_Event* event) final;
        void DisplayAsset() final;

    private:
        function<void()> YesClick, NoClick;

        Button *YesButton = nullptr, *NoButton = nullptr;
        TextBox* Prompt = nullptr;
        bool ButtonFlashing = 0;
    };

    vector<EventReceiver*> insiders;

    Particles* FlashingParticles = nullptr;
    Button* SaveGameButtons[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    Button* BackButton = nullptr;
    SDL_Texture* SaveFilePreviews[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    bool DisplayingPreview[5] = {0, 0, 0, 0, 0}, SaveFileExistance[5] = {0, 0, 0, 0, 0}, ButtonFlashing = 0;

    TextBox *title = nullptr, *preview = nullptr;
    ConfirmationBox* ConfDialog = nullptr;
};

#endif // NEWGAME_H
