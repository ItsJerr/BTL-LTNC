#include "charactermenu.h"
#include "globals.h"

bool PlayClassicClickAction() {
    GameEngine -> gGameData -> InDungeon = 1;
    GameEngine -> gGameData -> DungeonMode = CLASSICMODE;
    GameEngine -> gGameData -> DungeonLevel = 1;

    GameEngine -> gGameData -> CalculateModifiers();

    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(INGAMEID);
    SDL_PushEvent(&tmp);

    return 0;
}

bool PlayEndlessClickAction() {
    GameEngine -> gGameData -> InDungeon = 1;
    GameEngine -> gGameData -> DungeonMode = ENDLESSMODE;
    GameEngine -> gGameData -> DungeonLevel = 1;

    GameEngine -> gGameData -> CalculateModifiers();

    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(INGAMEID);
    SDL_PushEvent(&tmp);

    return 0;
}

bool ShopClickAction() {
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(SHOPID);
    tmp.user.data2 = new int(CHARACTERMENUID);
    SDL_PushEvent(&tmp);

    return 0;
}

bool MainMenuClickAction() {
    SDL_Event tmp; SDL_zero(tmp);
    tmp.type = ChangeModeEventID;
    tmp.user.data1 = new int(MAINMENUID);
    SDL_PushEvent(&tmp);

    return 0;
}

CharacterMenuLayer::CharacterMenuLayer() {
    if (!GameEngine -> gGameData -> GameDataLoaded) {
        cerr << "Error: Entering character menu without a character properly loaded";
        assert(false);
    }

    insiders.push_back(new Particles({100, 100, 100, 255}));

    Buttons[0] = new Button("play classic", {600, 300, 360, 75}, 3, PlayClassicClickAction, &DisplayControl[0], &DisplayControl[1]);
    ButtonDescriptions[0] = new TextBox("begin adventure", {-10, 830, 1570, 75}, 5, &DisplayControl[1], nullptr, &DisplayControl[0]);

    if (GameEngine -> gGameData -> MainGameCompleted) {
        Buttons[1] = new Button("play endless", {600, 381, 360, 75}, 3, PlayEndlessClickAction, &DisplayControl[0],
                                &DisplayControl[2]);
        ButtonDescriptions[1] = new TextBox("begin adventure", {-10, 830, 1570, 75}, 3, &DisplayControl[2]);
    }
    else {
        Buttons[1] = new TextBox("play endless", {600, 381, 360, 75}, 3, nullptr, &DisplayControl[2]);
        Buttons[1] -> SetColor(white, black, offwhite);
        ButtonDescriptions[1] = new TextBox("unlocks after completing classic mode once", {-10, 830, 1570, 75}, 3,
                                            &DisplayControl[2], nullptr, &DisplayControl[0]);
    }

    Buttons[2] = new Button("shop", {600, 462, 360, 75}, 3, ShopClickAction, &DisplayControl[0], &DisplayControl[3]);
    ButtonDescriptions[2] = new TextBox("enter the shop", {-10, 830, 1570, 75}, 3, &DisplayControl[3], nullptr, &DisplayControl[0]);

    Buttons[3] = new Button("main menu", {600, 543, 360, 75}, 3, MainMenuClickAction, &DisplayControl[0], &DisplayControl[4]);
    ButtonDescriptions[3] = new TextBox("back to main menu", {-10, 830, 1570, 75}, 3, &DisplayControl[4], nullptr,
                                        &DisplayControl[0]);

    for (int i = 0; i < 4; ++i) {
        insiders.push_back(Buttons[i]);
        insiders.push_back(ButtonDescriptions[i]);
    }
}

CharacterMenuLayer::~CharacterMenuLayer() {
    for (EventReceiver* asset : insiders) delete asset;
    insiders.clear();
}

void CharacterMenuLayer::HandleEvent(const SDL_Event* event) {
    for (EventReceiver* asset : insiders) asset -> HandleEvent(event);
}

void CharacterMenuLayer::Display() {
    for (EventReceiver* asset : insiders) asset -> DisplayAsset();
}
