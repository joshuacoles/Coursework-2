#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <printf.h>
#include "grid.h"
#include "helpers.h"

PosList allocPosList(int capacity) {
    assert(capacity > 0);

    return (PosList) {
            .length = 0,
            .capacity = capacity,
            .data = malloc(sizeof(Pos) * capacity)
    };
}

void freePosList(PosList list) {
    free(list.data);
}

void printPosList(char *prefix, PosList const *list) {
    for (int i = 0; i < list->length; ++i) {
        printf("%s(%d, %d)\n", prefix, list->data[i].x, list->data[i].y);
    }
}

// Append Pos to PosList, updating the length, re-allocating if need be.
void appendToPosList(PosList *list, Pos pos) {
    // If a PosList is too small, reallocate, double in size.
    if (list->length == list->capacity) {
        list->data = realloc(list->data, sizeof(Pos) * list->capacity * 2);
        list->capacity = list->capacity * 2;
    }

    list->data[list->length++] = pos;
}

bool containsPos(PosList const *l, Pos r) {
    for (int i = 0; i < l->length; ++i) {
        if (posEq(l->data[i], r)) return true;
    }

    return false;
}

int randomUniform(int r0, int r1) {
    return (rand() % (r1 - r0)) + r0;
}

bool randomBool(double p) {
    return (rand() / (double) RAND_MAX) < p;
}

int max(int a, int b) {
    return a > b ? a : b;
}
