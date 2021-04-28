#include <printf.h>
#include <assert.h>
#include <math.h>
#include "grid.h"
#include "helpers.h"
#include "cluster_finder.h"
#include "pos.h"

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint) {
    PosList cluster = allocPosList(grid->cells);
    PosList nextToProcess = allocPosList(grid->cells);
    PosList found = allocPosList(grid->cells);

    // Ensure initialPoint is within grid.
    assert(positionInBounds(*grid, initialPoint));

    appendToPosList(&cluster, initialPoint);
    appendToPosList(&nextToProcess, initialPoint);

    return (ClusterFinder) {
            .grid = grid,
            .initialPoint = initialPoint,
            .cluster = cluster,
            .nextToProcess = nextToProcess,
            .found = found
    };
}

ClusterFinder newClusterFinder(Grid *grid) {
    Pos pos;

    int c = 0;

    do {
        pos.x = randomUniform(0, grid->x_dim);
        pos.y = randomUniform(0, grid->y_dim);
        pos.z = randomUniform(0, grid->z_dim);

        if (c++ == 100) {
            fprintf(stderr,
                    "Warning, attempted to select a non-insulator as seed start, however failed after 100 attempts.");
        }
    } while (cellTypeOf(*grid, pos) == INSULATOR && c < 100);

    return newClusterFinderWithInitialPoint(grid, pos);
}

void freeClusterFinder(ClusterFinder clusterFinder) {
    freePosList(clusterFinder.cluster);
    freePosList(clusterFinder.nextToProcess);
    freePosList(clusterFinder.found);
    // DO NOT free grid, we do not own that memory
}

const int MAX_REACHABLE_SIZE = 26;

bool testCandidate(Grid grid, Pos from, CellType fromType, int dx, int dy, int dz) {
    // Ignore current position
    if (dx == 0 && dy == 0 && dz == 0) return false;
    int sig = 3 - (dx == 0) - (dy == 0) - (dz == 0);

    Pos candidate = offsetPosition(from, dx, dy, dz);
    if (!positionInBounds(grid, candidate)) return false;

    CellType candidateType = cellTypeOf(grid, candidate);

    int fromStrength = strengthOf(fromType);
    int candidateStrength = strengthOf(candidateType);

    // If either are zero, no connection can form.
    if (fromStrength * candidateStrength != 0) {
        // We use the maximum function to represent bi-directionality (ie can connect if a or b can)
        int maxStrength = max(fromStrength, candidateStrength);

        if (sig <= maxStrength) {
            return true;
        }
    }

    return false;
}

int findReachable(Grid grid, Pos from, Pos *out) {
    CellType fromType = cellTypeOf(grid, from);

    // An insulator cannot connect to or from anything.
    if (fromType == INSULATOR) return 0;

    int length = 0;

    // If we know this is a 2D grid, existing in a 3D, we can skip all `z` iteration.
    if (grid.is2D) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (testCandidate(grid, from, fromType, i, j, 0)) {
                    out[length++] = offsetPosition(from, i, j, 0);
                }
            }
        }
    } else {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                for (int k = -1; k <= 1; ++k) {
                    if (testCandidate(grid, from, fromType, i, j, k)) {
                        out[length++] = offsetPosition(from, i, j, k);
                    }
                }
            }
        }
    }

    return length;
}

void performSearchStep(ClusterFinder *self) {
//    printf("\tPerforming step\n");
//    printf("\t\tCluster:\n");
//    printPosList("\t\t\t", &self->cluster);
//    printf("\t\tNTP:\n");
//    printPosList("\t\t\t", &self->nextToProcess);
//    printf("\t\tFound (should be empty):\n");
//    printPosList("\t\t\t", &self->found);

    Pos *reachableCache = malloc(sizeof(Pos) * MAX_REACHABLE_SIZE);
    for (int i = 0; i < self->nextToProcess.length; ++i) {
        Pos pos = self->nextToProcess.data[i];
        int reachableFrom = findReachable(*self->grid, pos, reachableCache);
//        printf("\t\tReachable From: %d\n", reachableFrom);

        for (int j = 0; j < reachableFrom; ++j) {
            if (!containsPos(&self->cluster, reachableCache[j]) /*&&
                areReachablePointsConnected(*self->grid, pos, reachableCache[j])*/) {
                appendToPosList(&self->cluster, reachableCache[j]);
                appendToPosList(&self->found, reachableCache[j]);
            }
        }
    }

//    printf("\t\tNewly Found:\n");
//    printPosList("\t\t\t", &self->found);

//    printCluster(self);

    // Swap lists, relies on having compatible capacities.
    PosList mv = self->nextToProcess;
    self->nextToProcess = self->found;
    self->found = mv;

    // Reset found to zero, will ignore old elements
    self->found.length = 0;
}

void performSearch(ClusterFinder *self) {
    while (self->nextToProcess.length != 0) {
        performSearchStep(self);
    }
}

bool didFormPath(ClusterFinder *self) {
    bool connectedTop = false;
    bool connectedBottom = false;

    for (int i = 0; i < self->cluster.length; ++i) {
        Pos pos = self->cluster.data[i];

        if (pos.y == self->grid->y_dim - 1) {
            connectedBottom = true;
        }

        // These are two separate conditions as for `y_dim = 1`, a partical can be both.
        if (pos.y == 0) {
            connectedTop = true;
        }
    }

    return connectedBottom && connectedTop;
}

void printCluster(ClusterFinder *clusterFinder) {
    printf("Cluster: \n");
    Grid grid = *clusterFinder->grid;

    for (int z = 0; z < grid.z_dim; ++z) {
        fprintf(stdout, "(z = %d)\n", z);

        for (int j = 0; j < grid.y_dim; ++j) {
            for (int i = 0; i < grid.x_dim; ++i) {
                Pos pos = (Pos) {i, j, z};

                bool isInitialPoint = posEq(clusterFinder->initialPoint, pos);
                bool inCluster = containsPos(&clusterFinder->cluster, pos);

                if (isInitialPoint) {
                    fprintf(stdout, RED);
                } else if (inCluster) {
                    fprintf(stdout, MAG);
                }

                CellType cellType = cellTypeOf(*clusterFinder->grid, pos);

                fprintf(stdout, "%c", charOf(cellType));

                fprintf(stdout, RESET);
            }

            printf("\n");
        }

        printf("\n");
    }
}
