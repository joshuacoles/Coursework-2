#ifndef COURSEWORK_2_C_GRID_H
#define COURSEWORK_2_C_GRID_H

#include <stdlib.h>

// Since we only refer to `CellType`s by the enum constants, we set their values to be the printed characters for
// convenience.
typedef enum CellType {
    INSULATOR = '.',
    CONDUCTOR = '+',
    SUPER_CONDUCTOR = '*',
} CellType;

// Grid is pointer to a contiguous memory block (`data`), with associated size information.
// It can be passed by value or reference as all fields should be treated as immutable.
// Data is stored in row major format, with the origin being in the top left hand corner.
typedef struct Grid {
    int x_dim;
    int y_dim;

    CellType *data; // sizeof(CellType) * x_dim * y_dim
} Grid;

typedef struct Pos {
    int x;
    int y;
} Pos;

bool posEq(Pos a, Pos b);

CellType *cellTypeOf(Grid grid, Pos p);

Grid allocateGrid(int x_dim, int y_dim);
void freeGrid(Grid grid);

void fillGrid(Grid grid, int n, double pSuper);
void printGrid(Grid grid);

// See definition for documentation
int directlyReachableFrom(Grid grid, Pos pos, Pos *out);

#endif // COURSEWORK_2_C_GRID_H
