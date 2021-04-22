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

void directlyReachableFrom(Grid grid, int along_x, int along_y, int* out);
