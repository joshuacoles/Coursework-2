#include <stdlib.h>
#include <printf.h>
#include <assert.h>
#include <libc.h>

#include "helpers.h"
#include "grid.h"

Grid allocateGrid(int x_dim, int y_dim) {
    // We avoid overflow, and hence UB, by simply restricting the valid values of `x_dim` and `y_dim`.
    // The maximum delta of any connection is +/- 1, hence we restrict to [0, INT32_MAX - 1] so delta-ed range becomes
    // [-1, INT32_MAX] which avoids overflow. We perform bounds checks on this to ensure the result is in
    // [0, x_dim/y_dim] at computation.
    assert(0 < x_dim && x_dim < INT32_MAX);
    assert(0 < y_dim && y_dim < INT32_MAX);

    // Allocate backing array, initially its contents is undefined (although in practice zeroed). Due to our `CellType`
    // representation however `0` is not a valid value so we memset to `INSULATOR` to ensure validity.
    CellType *data = malloc(sizeof(CellType) * x_dim * y_dim);
    memset(data, INSULATOR, sizeof *data);

    return (Grid) {
            .x_dim = x_dim,
            .y_dim = y_dim,
            .data = data
    };
}

void freeGrid(Grid grid) {
    free(grid.data);
}

void fillGrid(Grid grid, int n, double pSuper) {
    int cells = grid.x_dim * grid.y_dim;
    assert(0 <= n && n <= cells);

    int nn = 0;

    for (int i = 0; i < cells; ++i) {
        grid.data[i] = INSULATOR;
    }

    while (nn < n) {
        int pos = randomUniform(0, grid.x_dim * grid.y_dim);
        if (grid.data[pos] == INSULATOR) {
            if (randomBool(pSuper)) {
                grid.data[pos] = SUPER_CONDUCTOR;
            } else {
                grid.data[pos] = CONDUCTOR;
            }

            nn++;
        }
    }
}

void printGrid(Grid grid) {
    for (int j = 0; j < grid.y_dim; ++j) {
        for (int i = 0; i < grid.x_dim; ++i) {
            fprintf(stdout, "%c", *cellTypeOf(grid, (Pos) {i, j}));
        }

        printf("\n");
    }
}

CellType *cellTypeOf(Grid grid, Pos p) {
    assert(positionInBounds(grid, p));
    int linearIndex = p.x + (p.y * grid.x_dim);
//    printf("LI: %d < %d\n", linearIndex, grid.x_dim * grid.y_dim);

    return &grid.data[linearIndex];
}

Pos offsetPosition(Pos a, int dx, int dy) {
    return (Pos) {
            .x = a.x + dx,
            .y = a.y + dy
    };
}

bool posEq(Pos a, Pos b) {
    return a.x == b.x && a.y == b.y;
}

bool positionInBounds(Grid grid, Pos pos) {
    return !((pos.x < 0 || pos.x >= grid.x_dim) ||
             (pos.y < 0 || pos.y >= grid.y_dim));
}

char charOf(CellType cellType) {
    switch (cellType) {
        case INSULATOR:
            return '.';
        case CONDUCTOR:
            return '+';
        case SUPER_CONDUCTOR:
            return '*';
        default:
            fprintf(stderr, "Error, unknown CellType\n");
            return 'E';
    }
}