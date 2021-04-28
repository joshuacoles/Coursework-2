#ifndef COURSEWORK_2_C_POS_H
#define COURSEWORK_2_C_POS_H

#include <stdbool.h>

/*
 * Pos functions
 * */

typedef struct Pos {
    int x;
    int y;
    int z;
} Pos;

bool posEq(Pos a, Pos b);
Pos offsetPosition(Pos a, int dx, int dy, int dz);

#endif //COURSEWORK_2_C_POS_H
