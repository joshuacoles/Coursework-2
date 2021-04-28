#include <stdlib.h>
#include <printf.h>
#include <assert.h>
#include <libc.h>

#include "helpers.h"
#include "grid.h"

Grid allocateGrid3D(int x_dim, int y_dim, int z_dim) {
    int cells = x_dim * y_dim * z_dim;

    // We avoid overflow, and hence UB, by simply restricting the valid values of `XX_dim`.
    // The maximum delta of any connection is +/- 1, hence we restrict to [0, INT32_MAX - 1] so delta-ed range becomes
    // [-1, INT32_MAX] which avoids overflow. We perform bounds checks on this to ensure the result is in
    // [0, XX_dim] at computation.
    assert(0 < x_dim && x_dim < INT32_MAX);
    assert(0 < y_dim && y_dim < INT32_MAX);
    assert(0 < z_dim && z_dim < INT32_MAX);

    // Simple check for overflow on multiplication, not perfect but will work.
    assert(0 < cells && x_dim <= cells && y_dim <= cells && z_dim <= cells);

    CellType *data = malloc(sizeof(CellType) * cells);
    memset(data, INSULATOR, sizeof *data);

    return (Grid) {
            .data = data,
            .x_dim = x_dim,
            .y_dim = y_dim,
            .z_dim = z_dim,
            .cells = cells,
            .is2D = false
    };
}

Grid allocateGrid2D(int x_dim, int y_dim) {
    Grid grid = allocateGrid3D(x_dim, y_dim, 1);
    grid.is2D = true;

    return grid;
}

void freeGrid(Grid grid) {
    free(grid.data);
}

void fillGrid(Grid grid, int n, double pSuper) {
    assert(0 <= n && n <= grid.cells);

    int nn = 0;

    for (int i = 0; i < grid.cells; ++i) {
        grid.data[i] = INSULATOR;
    }

    while (nn < n) {
        int pos = randomUniform(0, grid.cells);
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
    for (int z = 0; z < grid.z_dim; ++z) {
        fprintf(stdout, "(z = %d)\n", z);

        for (int j = 0; j < grid.y_dim; ++j) {
            for (int i = 0; i < grid.x_dim; ++i) {
                fprintf(stdout, "%c", charOf(cellTypeOf(grid, (Pos) {i, j, z})));
            }

            printf("\n");
        }

        printf("\n");
    }
}

int linearIndex(Grid grid, Pos p) {
    assert(positionInBounds(grid, p));
    return (p.z * grid.x_dim * grid.y_dim) + (p.y * grid.x_dim) + p.x;
}

CellType cellTypeOf(Grid grid, Pos p) {
    return grid.data[linearIndex(grid, p)];
}

Pos offsetPosition(Pos a, int dx, int dy, int dz) {
    return (Pos) {
            .x = a.x + dx,
            .y = a.y + dy,
            .z = a.z + dz,
    };
}

bool posEq(Pos a, Pos b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool positionInBounds(Grid grid, Pos pos) {
    return !((pos.x < 0 || pos.x >= grid.x_dim) ||
             (pos.y < 0 || pos.y >= grid.y_dim) ||
             (pos.z < 0 || pos.z >= grid.z_dim));
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

int strengthOf(CellType cellType) {
    switch (cellType) {
        case INSULATOR:
            return 0;
        case CONDUCTOR:
            return 1;
        case SUPER_CONDUCTOR:
            return 2;

        default:
            // Invalid CellType value
            fprintf(stderr, "Invalid CellType '%c'\n", cellType);

            return -1;
    }
}