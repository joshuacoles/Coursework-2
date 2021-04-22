#include <stdlib.h>
#include <time.h>

#include "grid.h"

int main() {
    srand(time(NULL));

    int x_dim = 10;
    int y_dim = 10;

    Grid grid = allocateGrid(x_dim, y_dim);
    fillGrid(grid, 10, 0);
    printGrid(grid);
    freeGrid(grid);

    return 0;
}
