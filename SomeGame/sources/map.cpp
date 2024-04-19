#include "map.h"
#include "globals.h"
#include "engine.h"
#include "actor.h"

using namespace std;

// check whether two rooms intersect. coordinates are increased by 2 in all directions to account for border.
bool intersect(int ulx, int uly, int lx, int ly, const tuple<int, int, int, int>& other) {
    int drx = ulx + lx + 1; int dry = uly + ly + 1; ulx -= 2; uly -= 2;
    int oulx, ouly, odrx, odry; tie(oulx, ouly, odrx, odry) = other;
    odrx += oulx + 1; odry += ouly + 1; oulx -= 2; ouly -= 2;

    if (drx < oulx || odrx < ulx) return false;
    if (dry < ouly || odry < uly) return false;
    return true;
}

// center of the room
pair<int, int> center(const tuple<int, int, int, int>& cur) {
    return make_pair(get<0>(cur) + get<2>(cur) / 2, get<1>(cur) + get<3>(cur) / 2);
}

// manhattan distance
int manhattan(pair<int, int> x, pair<int, int> y) {
    return abs(x.first - y.first) + abs(x.second - y.second);
}

// add monster at a certain coordinate. will rework later to port a list of monsters
void DungeonMap::AddMonster(int x, int y) {
    if (rnd(1, 100) < 80) {
        gEngine -> actors.push_back(new Actor("Slime", x, y, 40, 11));
        gEngine -> actors.back() -> combat = new MonsterCombatStat(15, 3, 2);
        gEngine -> actors.back() -> ai = new MonsterAI();
    }
    else {
        gEngine -> actors.push_back(new Actor("Dragonling", x, y, 42, 11));
        gEngine -> actors.back() -> combat = new MonsterCombatStat(15, 3, 4);
        gEngine -> actors.back() -> ai = new MonsterAI();
    }
}

// add item at a certain coordinate. will rework later to port a list of items
void DungeonMap::AddItem(int x, int y) {
    int roll = rnd(1, 100);
    if (roll < 50) {
        gEngine -> actors.push_back(new Actor("Small Health Potion", x, y, 3, 18));
        gEngine -> actors.back() -> blocks = 0;
        gEngine -> actors.back() -> pickable = new Healer(5);
    }
    else if (roll < 70) {
        gEngine -> actors.push_back(new Actor("Medium Health Potion", x, y, 4, 18));
        gEngine -> actors.back() -> blocks = 0;
        gEngine -> actors.back() -> pickable = new Healer(10);
    }
    else if (roll < 90) {
        gEngine -> actors.push_back(new Actor("Large Health Potion", x, y, 5, 18));
        gEngine -> actors.back() -> blocks = 0;
        gEngine -> actors.back() -> pickable = new Healer(20);
    }
    else {
        gEngine -> actors.push_back(new Actor("Scroll of Fireball", x, y, 1, 34));
        gEngine -> actors.back() -> blocks = 0;
        gEngine -> actors.back() -> pickable = new Fireball();
    }
}

// add gear at a certain coordinate. will rework later to port a list of gears
void DungeonMap::AddGear(int x, int y, bool type) {
    int roll = rnd(1, 100);
    if (type == 0) { // weapon
        if (roll < 50) {
            gEngine -> actors.push_back(new Actor("Bow", x, y, 26, 23));
            gEngine -> actors.back() -> blocks = 0;
            gEngine -> actors.back() -> pickable = new Gear(0, 3);
        }
        else {
            gEngine -> actors.push_back(new Actor("Axe", x, y, 42, 22));
            gEngine -> actors.back() -> blocks = 0;
            gEngine -> actors.back() -> pickable = new Gear(0, 4);
        }
    }
    else {
        if (roll < 50) {
            gEngine -> actors.push_back(new Actor("Light Armor", x, y, 32, 24));
            gEngine -> actors.back() -> blocks = 0;
            gEngine -> actors.back() -> pickable = new Gear(1, 3);
        }
        else {
            gEngine -> actors.push_back(new Actor("Chainmail", x, y, 33, 24));
            gEngine -> actors.back() -> blocks = 0;
            gEngine -> actors.back() -> pickable = new Gear(1, 4);
        }
    }
}

// generate maps using a primitive "digging" method: dig non-intersecting rooms in an originally all-wall map
DungeonMap::DungeonMap(int mapx, int mapy, int roomcnt, int ItemCount, int MonsterCount): mapx(mapx), mapy(mapy), roomcnt(roomcnt) {
    Map = new Tile*[mapx];
    for (int i = 0; i < mapx; ++i) Map[i] = new Tile[mapy];

    const int sidemin = 5, sidemax = 12;

    for (int i = 0; i < roomcnt; ++i) {
        // generate side lengths
        int lx = rnd(sidemin, sidemax);
        int ly = rnd(sidemin, sidemax);
        // generate coordinates
        int crx = rnd(1, mapx - lx - 1);
        int cry = rnd(1, mapy - ly - 1);

        bool inter = 0;
        for (const tuple<int, int, int, int>& other : rooms)
            inter |= intersect(crx, cry, lx, ly, other);

        if (!inter) rooms.emplace_back(crx, cry, lx, ly);
    }

    // mark all tiles in room as floor tiles
    for (const tuple<int, int, int, int>& current : rooms) {
        int x, y, lx, ly; tie(x, y, lx, ly) = current;
        for (int i = 0; i < lx; ++i) for (int j = 0; j < ly; ++j) {
            Map[x + i][y + j].IsWall = 0;
        }
    }

    // a similar approach to Prim's to connect all rooms. considering Manhattan distance
    bool* connected = new bool[rooms.size()];
    fill(connected, connected + rooms.size(), 0);

    pair<int, int>* dist = new pair<int, int>[rooms.size()]; // pair is used to memorize the closest room
    fill(dist, dist + rooms.size(), make_pair(INT_MAX, -1));
    dist[0] = make_pair(0, 0);

    auto connect = [&](int x, int y) {
        if (x == y) return;
        int cxx, cxy; tie(cxx, cxy) = center(rooms[x]);
        int cyx, cyy; tie(cyx, cyy) = center(rooms[y]);

        if (rnd(0, 1)) { // row first
            for (int i = min(cxy, cyy); i <= max(cxy, cyy); ++i) Map[cxx][i].IsWall = 0;
            for (int i = min(cxx, cyx); i <= max(cxx, cyx); ++i) Map[i][cyy].IsWall = 0;
        }
        else {
            for (int i = min(cxx, cyx); i <= max(cxx, cyx); ++i) Map[i][cxy].IsWall = 0;
            for (int i = min(cxy, cyy); i <= max(cxy, cyy); ++i) Map[cyx][i].IsWall = 0;
        }
    };

    int cnt = 0;
    while (cnt < rooms.size()) {
        pair<int, int> mini = make_pair(INT_MAX, -1);
        for (int i = 0; i < rooms.size(); ++i) if (!connected[i])
            mini = min(mini, make_pair(dist[i].first, i));

        connected[mini.second] = 1;
        connect(mini.second, dist[mini.second].second);

        for (int i = 0; i < rooms.size(); ++i) if (!connected[i])
            dist[i] = min(dist[i], make_pair(manhattan(center(rooms[i]), center(rooms[mini.second])), mini.second));

        ++cnt;
    }

    // add a couple of random corridors to minimize backtracking time
    int numpath = 5;

    for (int i = 0; i < numpath; ++i) {
        int x = rnd(0, rooms.size() - 1);
        int y = rnd(0, rooms.size() - 1);
        while (x == y) {
            x = rnd(0, rooms.size() - 1);
            y = rnd(0, rooms.size() - 1);
        }

        connect(x, y);
    }

    delete connected;
    delete dist;

    while (MonsterCount--) {
        int room = rnd(1, rooms.size() - 1);
        int x = rnd(get<0>(rooms[room]), get<0>(rooms[room]) + get<2>(rooms[room]) - 1);
        int y = rnd(get<1>(rooms[room]), get<1>(rooms[room]) + get<3>(rooms[room]) - 1);
        while (!CanPlace(x, y)) {
            x = rnd(get<0>(rooms[room]), get<0>(rooms[room]) + get<2>(rooms[room]) - 1);
            y = rnd(get<1>(rooms[room]), get<1>(rooms[room]) + get<3>(rooms[room]) - 1);
        }
        AddMonster(x, y);
    }

    while (ItemCount--) {
        int room = rnd(0, rooms.size() - 1);
        int x = rnd(get<0>(rooms[room]), get<0>(rooms[room]) + get<2>(rooms[room]) - 1);
        int y = rnd(get<1>(rooms[room]), get<1>(rooms[room]) + get<3>(rooms[room]) - 1);
        while (!CanPlace(x, y)) {
            x = rnd(get<0>(rooms[room]), get<0>(rooms[room]) + get<2>(rooms[room]) - 1);
            y = rnd(get<1>(rooms[room]), get<1>(rooms[room]) + get<3>(rooms[room]) - 1);
        }
        AddItem(x, y);
    }

    for (int i = 0; i < 2; ++i) {
        int room = rnd(0, rooms.size() - 1);
        int x = rnd(get<0>(rooms[room]), get<0>(rooms[room]) + get<2>(rooms[room]) - 1);
        int y = rnd(get<1>(rooms[room]), get<1>(rooms[room]) + get<3>(rooms[room]) - 1);
        while (!CanPlace(x, y)) {
            x = rnd(get<0>(rooms[room]), get<0>(rooms[room]) + get<2>(rooms[room]) - 1);
            y = rnd(get<1>(rooms[room]), get<1>(rooms[room]) + get<3>(rooms[room]) - 1);
        }
        AddGear(x, y, i);
    }
}

DungeonMap::~DungeonMap() {
    for (int i = 0; i < mapx; ++i) delete Map[i];
    delete Map;
}

pair<int, int> DungeonMap::GetStart() {
    return center(rooms[0]);
}

pair<int, int> DungeonMap::GetTarget() {
    // bfs to find furthest room from starting point

    int** dist = new int*[mapx];
    for (int i = 0; i < mapx; ++i) {
        dist[i] = new int[mapy];
        fill(dist[i], dist[i] + mapy, -1);
    }

    pair<int, int> st = GetStart();
    queue<pair<int, int>> q;
    q.push(st); dist[st.first][st.second] = 0;

    static const int r[] = {-1, 0, 0, 1}, c[] = {0, -1, 1, 0};

    while (q.size()) {
        int x, y; tie(x, y) = q.front(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = x + r[i], ny = y + c[i];
            if (nx < 0 || nx >= mapx || ny < 0 || ny >= mapy || Map[nx][ny].IsWall || ~dist[nx][ny]) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.push(make_pair(nx, ny));
        }
    }

    pair<int, pair<int, int>> mx = make_pair(0, make_pair(0, 0));
    for (const tuple<int, int, int, int> &room : rooms) mx = max(mx, make_pair(dist[center(room).first][center(room).second], center(room)));

    for (int i = 0; i < mapx; ++i) delete dist[i];
    delete dist;

    return mx.second;
}

// coordinates for actors are given relative to the player, as the player will always be at the center of the screen.
const int PlayerX = 29, PlayerY = 21;
void DungeonMap::Display(int px, int py) { // px, py are player coordinates in map
    for (int i = 0; i < mapx; ++i) for (int j = 0; j < mapy; ++j) {
        int relx = i - px, rely = j - py;
        if (relx < -29 || relx > 29 || relx < -21 || relx > 21) continue;

        if (Map[i][j].IsVisible) {
            SDL_Rect PositionInTileSet = {0, 0, 12, 12};
            SDL_Rect RenderTargetPosition = {(PlayerX + relx) * 21, (PlayerY + rely) * 21, 20, 20};
            if (Map[i][j].IsWall) {
                PositionInTileSet.x = 1;
                PositionInTileSet.y = 1;
            }
            else {
                PositionInTileSet.x = 1;
                PositionInTileSet.y = 27;
            }
            SDL_RenderCopy(gRenderer, TileSet, &PositionInTileSet, &RenderTargetPosition);
        }
        else if (Map[i][j].IsExplored) {
            SDL_Rect PositionInTileSet = {0, 0, 12, 12};
            SDL_Rect RenderTargetPosition = {(PlayerX + relx) * 21, (PlayerY + rely) * 21, 20, 20};
            if (Map[i][j].IsWall) {
                PositionInTileSet.x = 14;
                PositionInTileSet.y = 1;
            }
            else {
                PositionInTileSet.x = 14;
                PositionInTileSet.y = 27;
            }
            SDL_RenderCopy(gRenderer, TileSet, &PositionInTileSet, &RenderTargetPosition);
        }
    }
}

// FOV code ported with minor modifications from https://www.roguebasin.com/index.php/C++_shadowcasting_implementation
static const int multipliers[4][8] = {
    {1, 0, 0, -1, -1, 0, 0, 1},
    {0, 1, -1, 0, 0, -1, 1, 0},
    {0, 1, 1, 0, 0, -1, -1, 0},
    {1, 0, 0, 1, -1, 0, 0, -1}
};

void DungeonMap::cast_light(int x, int y, int radius, int row, float start_slope, float end_slope, int xx, int xy, int yx, int yy) {
    if (start_slope < end_slope) {
        return;
    }
    float next_start_slope = start_slope;
    for (int i = row; i <= radius; i++) {
        bool blocked = false;
        for (int dx = -i, dy = -i; dx <= 0; dx++) {
            float l_slope = (dx - 0.5) / (dy + 0.5);
            float r_slope = (dx + 0.5) / (dy - 0.5);
            if (start_slope < r_slope) {
                continue;
            } else if (end_slope > l_slope) {
                break;
            }

            int sax = dx * xx + dy * xy;
            int say = dx * yx + dy * yy;
            if ((sax < 0 && (int)abs(sax) > x) || (say < 0 && (int)abs(say) > y)) {
                continue;
            }
            int ax = x + sax;
            int ay = y + say;
            if (ax >= mapx || ay >= mapy) {
                continue;
            }

            int radius2 = radius * radius;
            if ((int)(dx * dx + dy * dy) < radius2) {
                Map[ax][ay].IsVisible = 1;
            }

            if (blocked) {
                if (Map[ax][ay].IsWall) {
                    next_start_slope = r_slope;
                    continue;
                } else {
                    blocked = false;
                    start_slope = next_start_slope;
                }
            } else if (Map[ax][ay].IsWall) {
                blocked = true;
                next_start_slope = r_slope;
                cast_light(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
            }
        }
        if (blocked) {
            break;
        }
    }
}

void DungeonMap::CalculateVisibility(int px, int py) {
    for (int i = 0; i < mapx; ++i) for (int j = 0; j < mapy; ++j) {
        Map[i][j].IsExplored |= Map[i][j].IsVisible;
        Map[i][j].IsVisible = 0;
    }
    for (int i = 0; i < 8; ++i)
        cast_light(px, py, 8, 1, 1.0, 0.0, multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i]);
    Map[px][py].IsVisible = 1;
    for (int i = 0; i < mapx; ++i) for (int j = 0; j < mapy; ++j) if (Map[i][j].IsVisible) {
        int dx = i - px;
        int dy = j - py;
        int distance = (int)sqrt(dx * dx + dy * dy);
        int NewScent = CurrentScent - distance;
        Map[i][j].scent = max(Map[i][j].scent, NewScent);
    }
}

bool DungeonMap::IsWall(int x, int y) {
    return Map[x][y].IsWall;
}

bool DungeonMap::CanWalk(int x, int y) {
    if (Map[x][y].IsWall) return false;
    for (Actor* actor : gEngine -> actors) {
        if (actor -> blocks && actor -> x == x && actor -> y == y) return false;
    }
    return true;
}

bool DungeonMap::CanPlace(int x, int y) {
    if (Map[x][y].IsWall) return false;
    for (Actor* actor : gEngine -> actors) {
        if (actor -> x == x && actor -> y == y) return false;
    }
    return true;
}

bool DungeonMap::IsInFov(int x, int y) {
    return Map[x][y].IsVisible;
}

int DungeonMap::GetScent(int x, int y) {
    return Map[x][y].scent;
}
