#include "loadcharacter.h"

LoadCharacterClass::~LoadCharacterClass() {
    for (int i = 0; i < 5; ++i) SDL_DestroyTexture(SaveFilePreviews[i]);
    for (EventReceiver* assets : insiders) delete assets;
    insiders.clear();
}

LoadCharacterClass::LoadCharacterClass() {
    insiders.push_back(FlashingParticles = new Particles({100, 100, 100, 255}));

    title = new TextBox("Choose a save file", {0, 0, SCREEN_WIDTH, 120}, transparent, 0, transparent, white);
    preview = new TextBox("Preview:", {450, 120, 1050, 700}, white, 5, black, white, nullptr, nullptr, 0);
    insiders.push_back(title); insiders.push_back(preview);

    int delta = 80;
    for (int i = 0; i < 5; ++i) {
        bool SaveFileExistance;
        tie(SaveFileExistance, SaveFilePreviews[i]) = PreviewSaveFile(i + 1);

        function<void()> OnClickFunction = [i]() {
            LoadGame(i + 1, gGameData);

            SDL_Event tmp; SDL_zero(tmp);
            tmp.type = ChangeModeEventID;
            tmp.user.data1 = new int(CHARACTERMAINMENUID);
            SDL_PushEvent(&tmp);
        };

        SaveGameButtons[i] = new Button("save slot #" + to_string(i + 1), {100, 270 + i * delta, 300, 75}, white, 3, black, (SaveFileExistance ? white : offwhite), offwhite, OnClickFunction, &ButtonFlashing, &DisplayingPreview[i], SaveFileExistance);

        insiders.push_back(SaveGameButtons[i]);
    }
}

void LoadCharacterClass::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    if (CURRENTMODE != LOADCHARACTERID) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void LoadCharacterClass::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
    for (int i = 0; i < 5; ++i) if (DisplayingPreview[i]) {
        SDL_Rect DstRect = {460, 180, 1040, 630};
        SDL_RenderCopy(gRenderer, SaveFilePreviews[i], nullptr, &DstRect);
    }
}
