#include "charactersaveload.h"

LoadCharacterLayer::~LoadCharacterLayer() {
    for (int i = 0; i < 5; ++i) SDL_DestroyTexture(SaveFilePreviews[i]);
    for (EventReceiver* assets : insiders) delete assets;
    insiders.clear();
}

LoadCharacterLayer::LoadCharacterLayer() {
    insiders.push_back(new Particles({100, 100, 100, 255}));

    insiders.push_back(new TextBox("Choose a save file", {0, 0, SCREEN_WIDTH, 120}, 0));
    dynamic_cast<TextBox*>(insiders.back()) -> SetColor(transparent, transparent, white);
    insiders.push_back(new TextBox("Preview:", {460, 120, 1040, 700}, 5));
    dynamic_cast<TextBox*>(insiders.back()) -> SetPosition(nullptr, 5, 5);

    insiders.push_back(new Button("back", {20, 20, 120, 50}, 3, []() -> bool {
        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(MAINMENUID);
        SDL_PushEvent(&tmp);
        return 0;
    }, &ButtonFlashing, nullptr));

    int delta = 81;
    for (int i = 0; i < 5; ++i) {
        bool SaveFileExistance;
        tie(SaveFileExistance, SaveFilePreviews[i]) = PreviewSaveFile(i + 1);

        if (SaveFileExistance) {
            function<bool()> OnClickFunction = [this, i]() {
                LoadGame(i + 1, gEngine -> gGameData);

                SDL_Event tmp; SDL_zero(tmp);
                tmp.type = ChangeModeEventID;
                tmp.user.data1 = new int(CHARACTERMENUID);
                SDL_PushEvent(&tmp);

                return 0;
            };

            insiders.push_back(new Button("save slot #" + to_string(i + 1), {100, 270 + i * delta, 300, 75}, 3, OnClickFunction,
                                            &ButtonFlashing, &DisplayingPreview[i]));
        }
        else {
            insiders.push_back(new TextBox("save slot #" + to_string(i + 1), {100, 270 + i * delta, 300, 75}, 3, nullptr,
                                           &DisplayingPreview[i], &ButtonFlashing));
            dynamic_cast<TextBox*>(insiders.back()) -> SetColor(white, black, offwhite);
        }
    }
}

void LoadCharacterLayer::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void LoadCharacterLayer::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
    for (int i = 0; i < 5; ++i) if (DisplayingPreview[i]) {
        SDL_Rect DstRect = {470, 180, 1040, 630};
        SDL_RenderCopy(gRenderer, SaveFilePreviews[i], nullptr, &DstRect);
    }
}

NewCharacterLayer::ConfirmationBox::ConfirmationBox(const int idx, NewCharacterLayer& parent) {
    parent.ButtonFlashing = 1;
    YesClick = [this, &parent, idx]() -> bool {
        parent.ButtonFlashing = 0;
        gEngine -> gGameData -> SaveFileIndex = idx;
        gEngine -> gGameData -> Wipe();
        SaveGame(gEngine -> gGameData);

        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(CHARACTERMENUID);
        SDL_PushEvent(&tmp);

        return 1;
    };
    NoClick = [this, &parent, idx]() -> bool {
        parent.ButtonFlashing = 0;

        vector<EventReceiver*>::iterator it = find(parent.insiders.begin(), parent.insiders.end(), this);
        if (it == parent.insiders.end()) {
            cerr << "Critical error: Failed to find ConfirmationBox iterator inside parents.insiders.";
            exit(0);
        }
        parent.insiders.erase(it);

        this -> ~ConfirmationBox();
        return 1;
    };

    YesButton = new Button("yes", {530, 510, 100, 70}, 3, YesClick, &ButtonFlashing, nullptr);
    YesButton -> SetColor(white, {0, 0, 0, 128}, white, offwhite);
    NoButton = new Button("no", {930, 510, 100, 70}, 3, NoClick, &ButtonFlashing, nullptr);
    NoButton -> SetColor(white, {0, 0, 0, 128}, white, offwhite);
    Prompt = new TextBox("are you sure you want to create a new character at save slot #" + to_string(idx) + "?",
                         {50, 400, 1460, 200}, 3);
    Prompt -> SetColor(white, {0, 0, 0, 128}, white);
    Prompt -> SetPosition(nullptr, 35, 20);
}

NewCharacterLayer::ConfirmationBox::~ConfirmationBox() {
    delete YesButton;
    delete NoButton;
    delete Prompt;
}

void NewCharacterLayer::ConfirmationBox::DisplayAsset() {
    Prompt -> DisplayAsset();
    YesButton -> DisplayAsset();
    NoButton -> DisplayAsset();
}

bool NewCharacterLayer::ConfirmationBox::HandleEvent(const SDL_Event* event) {
    if (YesButton -> HandleEvent(event)) return 1;
    if (NoButton -> HandleEvent(event)) return 1;
    return 0;
}

NewCharacterLayer::NewCharacterLayer() {
    insiders.push_back(new Particles({100, 100, 100, 255}));

    insiders.push_back(new TextBox("Choose a save slot", {0, 0, SCREEN_WIDTH, 120}, 0));
    dynamic_cast<TextBox*>(insiders.back()) -> SetColor(transparent, transparent, white);
    insiders.push_back(new TextBox("Preview:", {460, 120, 1040, 700}, 5));
    dynamic_cast<TextBox*>(insiders.back()) -> SetPosition(nullptr, 5, 5);

    insiders.push_back(new Button("back", {20, 20, 120, 50}, 3, []() -> bool {
        SDL_Event tmp; SDL_zero(tmp);
        tmp.type = ChangeModeEventID;
        tmp.user.data1 = new int(MAINMENUID);
        SDL_PushEvent(&tmp);
        return 0;
    }, &ButtonFlashing, nullptr));

    int delta = 81;
    for (int i = 0; i < 5; ++i) {
        SaveFilePreviews[i] = PreviewSaveFile(i + 1).second;

        function<bool()> OnClickFunction = [this, i]() {
            DisplayingPreview[i] = 0;
            insiders.push_back(new ConfirmationBox(i + 1, *this));
            return 0;
        };

        insiders.push_back(new Button("save slot #" + to_string(i + 1), {100, 270 + i * delta, 300, 75}, 3, OnClickFunction,
                                      &ButtonFlashing, &DisplayingPreview[i]));
    }
}

NewCharacterLayer::~NewCharacterLayer() {
    for (int i = 0; i < 5; ++i) SDL_DestroyTexture(SaveFilePreviews[i]);
    for (EventReceiver* assets : insiders) delete assets;
    insiders.clear();
}

void NewCharacterLayer::HandleEvent(const SDL_Event* event) {
    // Handle events for current mode
    for (const auto &Handler: insiders) Handler -> HandleEvent(event);
}

void NewCharacterLayer::Display() {
    // Display all assets
    for (const auto &asset: insiders) asset -> DisplayAsset();
    for (int i = 0; i < 5; ++i) if (DisplayingPreview[i]) {
        SDL_Rect DstRect = {470, 180, 1040, 630};
        SDL_RenderCopy(gRenderer, SaveFilePreviews[i], nullptr, &DstRect);
    }
}
