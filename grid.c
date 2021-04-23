#include <stdlib.h>
#include <printf.h>
#include <assert.h>

#include "helpers.h"
#include "grid.h"

int pos(Grid grid, Pos pos) {
    return pos.x + (pos.y * grid.y_dim);
}

CellType *idx(Grid grid, Pos p) {
    return &grid.data[pos(grid, p)];
}

Pos addP(Pos a, int dx, int dy) {
    return (Pos) {
            .x = a.x + dx,
            .y = a.y + dy
    };
}

Grid allocateGrid(int x_dim, int y_dim) {
    // To get around overflowing arithmetic, we just make it invalid.
    assert(0 < x_dim && x_dim < INT32_MAX);
    assert(0 < y_dim && y_dim < INT32_MAX);

    CellType *data = malloc(x_dim * y_dim * sizeof(CellType));

    return (Grid) {
            .x_dim = x_dim,
            .y_dim = y_dim,
            .data = data
    };
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

void freeGrid(Grid grid) {
    free(grid.data);
}

void printGrid(Grid grid) {
    for (int j = 0; j < grid.y_dim; ++j) {
        for (int i = 0; i < grid.x_dim; ++i) {
            fprintf(stdout, "%c", *idx(grid, (Pos) {i, j}));
        }

        printf("\n");
    }
}

// Remove element at `index` from `array`, updating `len`.
void removeElement(Pos *array, int index, int *len) {
    for (int i = index; i < *len - 1; i++) {
        array[i] = array[i + 1];
    }

    (*len)--;
}

// Returns the number of elements found, placed in oit
int directlyReachableFrom(Grid grid, Pos pos, Pos *out) {
    CellType type = *idx(grid, pos);
    int found = 0;

    switch (type) {
        case INSULATOR:
            return 0;

        case SUPER_CONDUCTOR:
            out[found] = addP(pos, 1, 1);
            found++;

            out[found] = addP(pos, -1, 1);
            found++;

            out[found] = addP(pos, 1, -1);
            found++;

            out[found] = addP(pos, -1, -1);
            found++;

            // Hey
        default:
            out[found] = addP(pos, 0, 1);
            found++;

            out[found] = addP(pos, 0, -1);
            found++;

            out[found] = addP(pos, 1, 0);
            found++;

            out[found] = addP(pos, -1, 0);
            found++;
    }

    // Remove invalid positions
    for (int i = 0; i < found; ++i) {
        if ((out[i].x < 0 || out[i].x >= grid.x_dim) ||
            (out[i].y < 0 || out[i].y >= grid.y_dim)) {
            removeElement(out, i, &found);

            i--;
        }
    }

    return found;
}
