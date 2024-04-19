#ifndef MAP_H
#define MAP_H

#include <bits/stdc++.h>
using namespace std;

struct Tile {
    bool IsWall = 1, IsExplored = 0, IsVisible = 0;
    int scent = 0;
};

const int mapx = 100, mapy = 100, // map dimensions
          roomcnt = 30; // number of rooms

class DungeonMap {
public:
    int CurrentScent = 20;

    DungeonMap(const int& MaxMonsters = 10, const int& MaxItems = 5);
    pair<int, int> GetStart();
    pair<int, int> GetTarget();

    bool IsWall(int x, int y);
    bool CanWalk(int x, int y);
    bool IsInFov(int x, int y);

    int GetScent(int x, int y);

    void CalculateVisibility(int px, int py);

    void Display(int px, int py);

private:
    Tile Map[mapx][mapy];
    vector<tuple<int, int, int, int>> rooms; // up-left coordinates and side lengths. does not include wall border.

    void cast_light(int x, int y, int radius, int row, float start_slope, float end_slope, int xx, int xy, int yx, int yy);

    bool CanPlace(int x, int y);
    void AddMonster(int x, int y);
    void AddItem(int x, int y);
};

#endif // MAP_H
