#ifndef COURSEWORK_2_C_GRID_H
#define COURSEWORK_2_C_GRID_H

#include <stdlib.h>

typedef enum CellType {
    INSULATOR = '.',
    CONDUCTOR = '+',
    SUPER_CONDUCTOR = '*',
} CellType;

typedef struct Grid {
    int x_dim;
    int y_dim;

    CellType *data;
} Grid;

typedef struct Pos {
    int x;
    int y;
} Pos;

Grid allocateGrid(int x_dim, int y_dim);
void fillGrid(Grid grid, int n, double pSuper);
void freeGrid(Grid grid);
void printGrid(Grid grid);

int directlyReachableFrom(Grid grid, Pos pos, Pos *out);

#endif // COURSEWORK_2_C_GRID_H
