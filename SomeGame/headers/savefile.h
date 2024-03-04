#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <fstream>
#include "gamevar.h"
#include "button.h"

using namespace std;

/// converts a variable name to a string cuz im too lazy LMAO
#define VarName(x) #x

/// load variable from file. also checks for its existence. returns false if there's an error.
template<typename T> bool LoadVariableFromFile(ifstream &InFile, T &Variable);

void LoadGame(const int FileIndex, GameData* DataLoc);

void SaveGame(const int FileIndex, GameData* DataLoc);

pair<bool, SDL_Texture*> PreviewSaveFile(const int FileIndex);

#endif // SAVEFILE_H
