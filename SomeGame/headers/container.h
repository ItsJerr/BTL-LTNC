#ifndef CONTAINER_H
#define CONTAINER_H

#include <bits/stdc++.h>
using namespace std;

class Actor;

class Container {
public:
    int size;
    vector<Actor*> inventory;

    Container(int size): size(size) {}
    ~Container();

    bool add(Actor* actor);
    void remove(Actor* actor);
};

#endif // CONTAINER_H
