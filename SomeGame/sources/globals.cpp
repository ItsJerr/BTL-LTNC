#include "globals.h"

// contains functions that I think are useful globally

// mersenne twister + high precision clock for seed. probably familiar to all cp-ers
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l, int r) {
    return l + rng() % (r - l + 1);
}

// random rounding, for example: 3.2 have a 80% chance of rounding to 3 and 20% chance of rounding to 4
int rround(float x) {
    const int prec = 1e6; // precision
    return floor(x) + 1 * (rnd(0, prec - 1) < prec * fmod(x, 1.0));
}

// generates a random float between a and b
uniform_real_distribution<float> frng(0.0, 1.0);
float frnd(float a, float b) {
    return a + (b - a) * frng(rng);
}
