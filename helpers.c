#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <printf.h>
#include "grid.h"
#include "helpers.h"

PosList allocPosList(int capacity) {
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

// Remove element at `index` from `array`, updating `len`.
void removeElement(Pos *array, int index, int *len) {
    for (int i = index; i < *len - 1; i++) {
        array[i] = array[i + 1];
    }

    (*len)--;
}