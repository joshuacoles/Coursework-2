#ifndef COURSEWORK_2_C_HELPERS_H
#define COURSEWORK_2_C_HELPERS_H

#include <stdbool.h>
#include "grid.h"

typedef struct PosList {
    Pos* data;
    int length;
    int capacity;
} PosList;

PosList allocPosList(int capacity);
void appendToPosList(PosList* list, Pos pos);

bool containsGen(void const *list, int len, void* item, bool (*eq)(void*,void*));

bool contains(int const *list, int len, int r);
bool containsPos(PosList const *list, Pos r);
bool randomBool(double p);
int randomUniform(int r0, int r1);

#endif // COURSEWORK_2_C_HELPERS_H
