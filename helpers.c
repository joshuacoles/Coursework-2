#include <stdbool.h>
#include <stdlib.h>
#include <printf.h>
#include "grid.h"
#include "helpers.h"

PosList allocPosList(int capacity) {
    return (PosList) {
        .length = 0,
        .capacity = capacity,
        .data = malloc(capacity * sizeof(Pos))
    };
}

void appendToPosList(PosList *list, Pos pos) {
    if (list->length == list->capacity) {
        printf("Realloc has not been implemented");
        exit(1);
    }

    list->data[list->length++] = pos;
}

// Todo make this generic with fn pointer for equality?
bool containsPos(PosList const *l, Pos r) {
    for (int i = 0; i < l->length; ++i) {
        if (l->data[i].x == r.x && l->data[i].y == r.y) return true;
    }

    return false;
}

int randomUniform(int r0, int r1) {
    return (rand() % (r1 - r0)) + r0;
}

bool randomBool(double p) {
    return (rand() / (double) RAND_MAX) < p;
}
