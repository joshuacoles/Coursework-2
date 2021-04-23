#include <stdbool.h>
#include <stdlib.h>
#include "grid.h"
#include "helpers.h"

PosList allocPosList(int capacity) {
    return (PosList) {
        .length = 0,
        .capacity = capacity,
        .data = malloc(capacity * sizeof(Pos))
    };
}

bool contains(int const *l, int len, int r) {
    for (int i = 0; i < len; ++i) {
        if (l[i] == r) return true;
    }

    return false;
}

// Todo make this generic with fn pointer for equality?
bool containsPos(Pos const *l, int len, Pos r) {
    for (int i = 0; i < len; ++i) {
        if (l[i].x == r.x && l[i].y == r.y) return true;
    }

    return false;
}

// Todo check usages for inclusive / exclusive on r0,r1
int randomUniform(int r0, int r1) {
    return (rand() % (r1 - r0)) + r0;
}

bool randomBool(double p) {
    return (rand() / (double) RAND_MAX) < p;
}
