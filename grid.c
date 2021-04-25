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
    size_t cells = grid.x_dim * grid.y_dim;
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
    int linearIndex = p.x + (p.y * grid.y_dim);

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

// FIXME this needs to be re-worked so we check for SC in corner even if we are currently on a Standard Cond.
int directlyReachableFrom(Grid grid, Pos pos, Pos *out) {
    CellType type = *cellTypeOf(grid, pos);
    int found = 0;

    switch (type) {
        case INSULATOR:
            return 0;

        case SUPER_CONDUCTOR:
            out[found++] = offsetPosition(pos, 1, 1);
            out[found++] = offsetPosition(pos, -1, 1);
            out[found++] = offsetPosition(pos, 1, -1);
            out[found++] = offsetPosition(pos, -1, -1);

        default:
            // Because of lack of break / return, this will match CONDUCTOR on default and SUPER_CONDUCTOR on
            // fallthrough.
            out[found++] = offsetPosition(pos, 0, 1);
            out[found++] = offsetPosition(pos, 0, -1);
            out[found++] = offsetPosition(pos, 1, 0);
            out[found++] = offsetPosition(pos, -1, 0);
    }

    // Remove positions which are outside the bounds of the grid.
    for (int i = 0; i < found; ++i) {
        if ((out[i].x < 0 || out[i].x >= grid.x_dim) ||
            (out[i].y < 0 || out[i].y >= grid.y_dim)) {
            // Remove the ith element, move succeeding elements back one. Also update index to i - 1 so we do not skip
            // over the new ith element when the ++i executes
            removeElement(out, i--, &found);
        }
    }

    return found;
}
