#include "headers/newcharacter.h"

NewCharacterClass::~NewCharacterClass() {
    for (int i = 0; i < 5; ++i) SDL_DestroyTexture(SaveFilePreviews[i]);
    for (EventReceiver* assets : insiders) delete assets;
    insiders.clear();
}

void SaveSlotButton::FlashEndAction() {
    LoadGame(SaveIndex, gGameData);

    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(CHARACTERMAINMENUID);
    SDL_PushEvent(&tmp);
}

NewCharacterClass::NewCharacterClass() {
    title = new TextBox("Choose a save file", {0, 0, SCREEN_WIDTH, 75}, transparent, 0, transparent, white);
    preview = new TextBox("Preview:", {700, 150, 870, 600}, white, 3, black, white);
    insiders.push_back(title); insiders.push_back(preview);

    insiders.push_back(FlashingParticles = new Particles(white));
    int delta = 80;
    for (int i = 0; i < 5; ++i) {
        bool SaveFileExistance;
        tie(SaveFileExistance, SaveFilePreviews[i]) = PreviewSaveFile(i + 1);
        SaveGameButtons[i] = new SaveSlotButton("save slot #" + to_string(i + 1), {250, 250 + i * delta, 300, 75}, white, 3, black, (SaveFileExistance ? white : offwhite), offwhite, i + 1, &ButtonFlashing, &DisplayingPreview[i], SaveFileExistance);
        insiders.push_back(SaveGameButtons[i]);
    }
}

void NewCharacterClass::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    if (CURRENTMODE != NEWCHARACTERID) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void NewCharacterClass::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
}
