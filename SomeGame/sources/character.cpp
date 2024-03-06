#include "character.h"

LoadCharacterClass::~LoadCharacterClass() {
    for (int i = 0; i < 5; ++i) SDL_DestroyTexture(SaveFilePreviews[i]);
    for (EventReceiver* assets : insiders) delete assets;
    insiders.clear();
}

LoadCharacterClass::LoadCharacterClass() {
    insiders.push_back(FlashingParticles = new Particles({100, 100, 100, 255}));

    title = new TextBox("Choose a save file", {0, 0, SCREEN_WIDTH, 120}, transparent, 0, transparent, white);
    preview = new TextBox("Preview:", {460, 120, 1040, 700}, white, 5, black, white, nullptr, nullptr, 0);
    insiders.push_back(title); insiders.push_back(preview);

    BackButton = new Button("back", {20, 20, 120, 50}, white, 3, black, white, offwhite, []() {
        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(MAINMENUID);
        SDL_PushEvent(&tmp);
    }, &ButtonFlashing, nullptr, 1);
    insiders.push_back(BackButton);

    int delta = 80;
    for (int i = 0; i < 5; ++i) {
        bool SaveFileExistance;
        tie(SaveFileExistance, SaveFilePreviews[i]) = PreviewSaveFile(i + 1);

        function<void()> OnClickFunction = nullptr;

        if (SaveFileExistance) OnClickFunction = [this, i]() {
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
        SDL_Rect DstRect = {470, 180, 1040, 630};
        SDL_RenderCopy(gRenderer, SaveFilePreviews[i], nullptr, &DstRect);
    }
}

NewCharacterClass::ConfirmationBox::ConfirmationBox(const int& idx, NewCharacterClass& parent) {
    parent.ButtonFlashing = 1;
    YesClick = [this, &parent, idx]() {
        parent.ButtonFlashing = 0;
        gGameData -> SaveFileIndex = idx;
        gGameData -> Wipe();
        SaveGame(gGameData);

        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(CHARACTERMAINMENUID);
        SDL_PushEvent(&tmp);

        parent.ConfDialog = nullptr;
        this -> ~ConfirmationBox();
    };
    NoClick = [this, &parent, idx]() {
        parent.ButtonFlashing = 0;

        parent.ConfDialog = nullptr;
        this -> ~ConfirmationBox();
    };

    YesButton = new Button("yes", {530, 510, 100, 70}, white, 3, {0, 0, 0, 128}, white, offwhite, YesClick, &ButtonFlashing, nullptr, 1);
    NoButton = new Button("no", {930, 510, 100, 70}, white, 3, {0, 0, 0, 128}, white, offwhite, NoClick, &ButtonFlashing, nullptr, 1);
    Prompt = new TextBox("are you sure you want to create a new character at save slot #" + to_string(idx) + "?", {50, 400, 1460, 200}, white, 3, {0, 0, 0, 128}, white, {85, 420});
}

NewCharacterClass::ConfirmationBox::~ConfirmationBox() {
    delete YesButton;
    delete NoButton;
    delete Prompt;
}

void NewCharacterClass::ConfirmationBox::DisplayAsset() {
    Prompt -> DisplayAsset();
    YesButton -> DisplayAsset();
    NoButton -> DisplayAsset();
}

bool NewCharacterClass::ConfirmationBox::HandleEvent(const SDL_Event* event) {
    if (YesButton -> HandleEvent(event)) return 1;
    if (NoButton -> HandleEvent(event)) return 1;
    return 0;
}

NewCharacterClass::NewCharacterClass() {
    insiders.push_back(FlashingParticles = new Particles({100, 100, 100, 255}));

    title = new TextBox("Choose a save slot", {0, 0, SCREEN_WIDTH, 120}, transparent, 0, transparent, white);
    preview = new TextBox("Preview:", {460, 120, 1040, 700}, white, 5, black, white, nullptr, nullptr, 0);
    insiders.push_back(title); insiders.push_back(preview);

    BackButton = new Button("back", {20, 20, 120, 50}, white, 3, black, white, offwhite, []() {
        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(MAINMENUID);
        SDL_PushEvent(&tmp);
    }, &ButtonFlashing, nullptr, 1);
    insiders.push_back(BackButton);

    int delta = 80;
    for (int i = 0; i < 5; ++i) {
        tie(SaveFileExistance[i], SaveFilePreviews[i]) = PreviewSaveFile(i + 1);

        function<void()> OnClickFunction = [this, i]() {
            ConfDialog = new ConfirmationBox(i + 1, *this);
        };

        SaveGameButtons[i] = new Button("save slot #" + to_string(i + 1), {100, 270 + i * delta, 300, 75}, white, 3, black, white, offwhite, OnClickFunction, &ButtonFlashing, &DisplayingPreview[i], 1);

        insiders.push_back(SaveGameButtons[i]);
    }
}

NewCharacterClass::~NewCharacterClass() {
    for (int i = 0; i < 5; ++i) SDL_DestroyTexture(SaveFilePreviews[i]);
    for (EventReceiver* assets : insiders) delete assets;
    insiders.clear();
}

void NewCharacterClass::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    if (CURRENTMODE != NEWCHARACTERID) return;

    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
    if (ConfDialog) ConfDialog -> HandleEvent(event);
}

void NewCharacterClass::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
    for (int i = 0; i < 5; ++i) if (DisplayingPreview[i]) {
        SDL_Rect DstRect = {470, 180, 1040, 630};
        SDL_RenderCopy(gRenderer, SaveFilePreviews[i], nullptr, &DstRect);
    }
    if (ConfDialog) ConfDialog -> DisplayAsset();
}
