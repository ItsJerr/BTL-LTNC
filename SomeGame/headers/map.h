#ifndef MAP_H
#define MAP_H

#include <bits/stdc++.h>
using namespace std;

struct Tile {
    bool IsWall = 1, IsExplored = 0, IsVisible = 0;
    int scent = 0;
};

class DungeonMap {
public:
    int CurrentScent = 20, mapx, mapy, roomcnt, level;

    DungeonMap(int level, int mapx, int mapy, int roomcnt, int HealerCount, int ScrollCount, int MonsterCount);
    ~DungeonMap();
    pair<int, int> GetStart();
    pair<int, int> GetTarget();

    bool IsWall(int x, int y);
    bool CanWalk(int x, int y);
    bool IsInFov(int x, int y);

    int GetScent(int x, int y);

    void CalculateVisibility(int px, int py);

    void Display(int px, int py);

private:
    Tile** Map;
    vector<tuple<int, int, int, int>> rooms; // up-left coordinates and side lengths. does not include wall border.

    void cast_light(int x, int y, int radius, int row, float start_slope, float end_slope, int xx, int xy, int yx, int yy);

    bool CanPlace(int x, int y);
    void AddMonster(int x, int y);
    void AddHealer(int x, int y);
    void AddScroll(int x, int y);
    void AddGear(int x, int y, bool type);
};

#endif // MAP_H
