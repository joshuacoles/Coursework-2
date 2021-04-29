#ifndef COURSEWORK_2_C_GRID_H
#define COURSEWORK_2_C_GRID_H

#include <stdlib.h>
#include <stdbool.h>
#include "pos.h"

/*
 * CellType
 * */

typedef unsigned char CellType;
const CellType INSULATOR, CONDUCTOR, SUPER_CONDUCTOR;

//typedef enum CellType {
//    INSULATOR = '.',
//    CONDUCTOR = '+',
//    SUPER_CONDUCTOR = '*',
//} CellType;

int strengthOf(CellType cellType);

char charOf(CellType cellType);

/*
 * Grid
 * */

// Grid is pointer to a contiguous memory block (`data`), with associated size information.
// It can be passed by value or reference as all fields should be treated as immutable.
// Data is stored in row major format, with the origin being in the top left hand corner.
// It is defined as a 3D data structure, however in the 2D case we will only use the z = 0 plane.
typedef struct Grid {
    int x_dim;
    int y_dim;
    int z_dim;

    int cells;

    CellType *data; // sizeof(CellType) * x_dim * y_dim
} Grid;

/*
 * Grid functions
 * */

Grid allocateGrid2D(int x_dim, int y_dim);

Grid allocateGrid3D(int x_dim, int y_dim, int z_dim);

void freeGrid(Grid grid);

void fillGrid(Grid grid, int n, double pSuper);

void printGrid(Grid grid);

CellType cellTypeOf(Grid grid, Pos p);

bool positionInBounds(Grid grid, Pos pos);

#endif // COURSEWORK_2_C_GRID_H
