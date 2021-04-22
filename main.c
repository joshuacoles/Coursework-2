#include <stdlib.h>
#include <time.h>
#include <printf.h>

#include "grid.h"

void q1() {
    int x_dim = 10;
    int y_dim = 10;
    int n_conductors = 3;
    int n_grids = 3;
    double pSuper = 0;

    Grid grid = allocateGrid(x_dim, y_dim);

    for (int i = 0; i < n_grids + 1; ++i) {
        // Fill completely regenerates grid so we can reuse the allocation
        fillGrid(grid, n_conductors, pSuper);
        printGrid(grid);
        printf("\n\n");
    }

    freeGrid(grid);
}

int main() {
    srand(time(NULL));

    q1();

    return 0;
}
