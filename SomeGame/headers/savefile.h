#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <fstream>
#include "gamevar.h"
#include "button.h"

using namespace std;

/// load variable from file. also checks for its existence. returns false if there's an error.
template<typename T> bool LoadVariableFromFile(ifstream &InFile, T &Variable);

/// load game into DataLoc.
void LoadGame(const int& FileIndex, GameData* DataLoc);

void SaveGame(const GameData* DataLoc);

pair<bool, SDL_Texture*> PreviewSaveFile(const int& FileIndex);

#endif // SAVEFILE_H
