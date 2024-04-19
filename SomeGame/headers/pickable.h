#ifndef PICKABLE_H
#define PICKABLE_H

#include <bits/stdc++.h>
using namespace std;

class Actor;

class Pickable {
public:
    bool pick(Actor* owner, Actor* wearer);
    virtual bool use(Actor* owner, Actor* wearer);
};

// implement items here
class Healer: public Pickable {
public:
    int amount;
    Healer(int amount): amount(amount) {}

    bool use(Actor* owner, Actor* wearer) override;
};

class Fireball: public Pickable {
public:
    bool use(Actor* owner, Actor* wearer) override;
};

class Confuser: public Pickable {
public:
    bool use(Actor* owner, Actor* wearer) override;
};

#endif // PICKABLE_H
