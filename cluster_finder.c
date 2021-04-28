#include <printf.h>
#include <assert.h>
#include "grid.h"
#include "helpers.h"
#include "cluster_finder.h"

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint) {
    PosList cluster = allocPosList(grid->x_dim * grid->y_dim);
    PosList nextToProcess = allocPosList(grid->x_dim * grid->y_dim);
    PosList found = allocPosList(grid->x_dim * grid->y_dim);

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
    int x;
    int y;

    int c = 0;

    do {
        x = randomUniform(0, grid->x_dim);
        y = randomUniform(0, grid->y_dim);
        if (c++ == 100) {
            fprintf(stderr,
                    "Warning, attempted to select a non-insulator as seed start, however failed after 100 attempts.");
        }
    } while (*cellTypeOf(*grid, (Pos) {x, y}) == INSULATOR && c < 100);

    return newClusterFinderWithInitialPoint(grid, (Pos) {x, y});
}

void freeClusterFinder(ClusterFinder clusterFinder) {
    freePosList(clusterFinder.cluster);
    freePosList(clusterFinder.nextToProcess);
    freePosList(clusterFinder.found);
    // DO NOT free grid, we do not own that memory
}

// This is stored as a constant to allow for (easier) 3D grids.
const int MAX_REACHABLE_SIZE = 8;

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

int findReachable(Grid grid, Pos from, Pos *out) {
    CellType fromType = *cellTypeOf(grid, from);

    // An insulator cannot connect to or from anything.
    if (fromType == INSULATOR) return 0;

    int length = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            // Ignore current position
            if (i == 0 && j == 0) continue;
            int sig = 2 - (i == 0) - (j == 0);

            Pos candidate = offsetPosition(from, i, j);
            if (!positionInBounds(grid, candidate)) continue;

            CellType candidateType = *cellTypeOf(grid, candidate);

            int fromStrength = strengthOf(fromType);
            int candidateStrength = strengthOf(candidateType);

            // If either are zero, no connection can form.
            if (fromStrength * candidateStrength != 0) {
                // We use the maximum function to represent bi-directionality (ie can connect if a or b can)
                int maxStrength = max(fromStrength, candidateStrength);

                if (sig <= maxStrength) {
                    out[length++] = candidate;
                }
            }

            /* todo doc this in report
             * Firstly we note can classify each delta (an (i,j) pair) by a signature, aka the number of zeros in
             * its delta. This is displayed schematically below:
             *
             *  0 1 0
             *  1 2 1
             *  0 1 0
             *
             * With conductors connecting to sigs >= 1 and super-conductors sigs >= 0 (obviously ignoring the center).
             *
             * With the intention of allowing this to easily generalise to higher dimensions however we adapt this logic
             * to be sig'(p) = Number of Dimensions - Number of Zeros, hence becoming,
             *
             *  2 1 2
             *  1 0 1
             *  2 1 2
             *
             * Where we ignore sig'(p) == 0 as the central point, then conductors are sig' <= 1, and super-condcutors
             * sig' <= 2. This would allow us to create a third class of conductor in 3D which could connect to cells
             * with a sig' of 3.
             */
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
//    printf("Starting search\n");
    while (self->nextToProcess.length != 0) {
        performSearchStep(self);
    }
//    printf("Ending search\n");
}

bool didFormPath(ClusterFinder *self) {
    bool connectedTop = false;
    bool connectedBottom = false;

    for (int i = 0; i < self->cluster.length; ++i) {
        Pos pos = self->cluster.data[i];
//        printf("Check (%d, %d)\n", pos.x, pos.y);

        if (pos.y == self->grid->y_dim - 1) {
//            printf("\t Con bottom (%d, %d)\n", pos.x, pos.y);
            connectedBottom = true;
        }

        if (pos.y == 0) {
//            printf("\t Con top (%d, %d)\n", pos.x, pos.y);
            connectedTop = true;
        }
    }

    return connectedBottom && connectedTop;
}

void printCluster(ClusterFinder *clusterFinder) {
    for (int j = 0; j < clusterFinder->grid->y_dim; ++j) {
        for (int i = 0; i < clusterFinder->grid->x_dim; ++i) {
            bool isInitialPoint = posEq(clusterFinder->initialPoint, (Pos) {i, j});
            bool inCluster = containsPos(&clusterFinder->cluster, (Pos) {i, j});

            if (inCluster) {
                fprintf(stdout, MAG);
            } else if (isInitialPoint) {
                fprintf(stdout, RED);
            }

            CellType cellType = *cellTypeOf(*clusterFinder->grid, (Pos) {i, j});

            fprintf(stdout, "%c", charOf(cellType));

            fprintf(stdout, RESET);
        }

        printf("\n");
    }
}
