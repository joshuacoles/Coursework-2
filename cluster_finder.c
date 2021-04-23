#include <printf.h>
#include <assert.h>
#include "grid.h"
#include "helpers.h"
#include "cluster_finder.h"

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint) {
    PosList cluster = allocPosList(grid->x_dim * grid->y_dim);
    PosList nextToProcess = allocPosList(grid->x_dim * grid->y_dim);
    PosList found = allocPosList(grid->x_dim * grid->y_dim);

    appendToPosList(&cluster, initialPoint);
    appendToPosList(&nextToProcess, initialPoint);

    return (ClusterFinder) {
            .grid = grid,
            .initialPoint = initialPoint,
            .cluster = cluster,
            .nextToProcess = nextToProcess,
            .found = found // todo explain
    };
}

ClusterFinder newClusterFinder(Grid *grid) {
    int x = randomUniform(0, grid->x_dim);
    int y = randomUniform(0, grid->y_dim);

    return newClusterFinderWithInitialPoint(grid, (Pos) {x, y});
}

void freeClusterFinder(ClusterFinder clusterFinder) {
    freePosList(clusterFinder.cluster);
    freePosList(clusterFinder.nextToProcess);
    freePosList(clusterFinder.found);
    // DO NOT free grid, we do not own that memory
}

const int MAX_REACHABLE_SIZE = 8;

// Requires that a <-> is reachable from at least one direction.
// Ie that a is a member of reachableFrom(b) or visa-versa.
bool areReachablePointsConnected(Grid grid, Pos a, Pos b) {
    CellType cellA = *cellTypeOf(grid, a);
    CellType cellB = *cellTypeOf(grid, b);

    return cellA != INSULATOR && cellB != INSULATOR;
}

void performSearchStep(ClusterFinder *self) {
//    printf("\tPerforming step\n");
//    printf("\t\tCluster:\n");
//    printPosList("\t\t\t", &self->cluster);
//    printf("\t\tNTP:\n");
//    printPosList("\t\t\t", &self->nextToProcess);
//    printf("\t\tShould be empty:\n");
//    printPosList("\t\t\t", &self->found);

    Pos *reachableCache = malloc(sizeof(Pos) * MAX_REACHABLE_SIZE);
    for (int i = 0; i < self->nextToProcess.length; ++i) {
        Pos pos = self->nextToProcess.data[i];
        int reachableFrom = directlyReachableFrom(*self->grid, pos, reachableCache);

        for (int j = 0; j < reachableFrom; ++j) {
            // TODO Add bidirectionality
            if (!containsPos(&self->cluster, reachableCache[j]) &&
                areReachablePointsConnected(*self->grid, pos, reachableCache[j])) {
                appendToPosList(&self->cluster, reachableCache[j]);
                appendToPosList(&self->found, reachableCache[j]);
            }
        }
    }

//    printf("\t\tNewly Found:\n");
//    printPosList("\t\t\t", &self->found);

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
        printf("Check (%d, %d)\n", pos.x, pos.y);

        if (pos.y == self->grid->y_dim - 1) {
            printf("\t Con bottom (%d, %d)\n", pos.x, pos.y);
            connectedBottom = true;
        }

        if (pos.y == 0) {
            printf("\t Con top (%d, %d)\n", pos.x, pos.y);
            connectedTop = true;
        }
    }

    return connectedBottom && connectedTop;
}

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void printCluster(ClusterFinder *clusterFinder) {
    for (int j = 0; j < clusterFinder->grid->y_dim; ++j) {
        for (int i = 0; i < clusterFinder->grid->x_dim; ++i) {
            bool isInitialPoint = posEq(clusterFinder->initialPoint, (Pos) {i, j});
            bool inCluster = containsPos(&clusterFinder->cluster, (Pos) {i, j});

            if (isInitialPoint) {
                fprintf(stdout, RED);
            } else if (inCluster) {
                fprintf(stdout, MAG);
            }

            CellType cellType = *cellTypeOf(*clusterFinder->grid, (Pos) {i, j});

            fprintf(stdout, "%c", cellType);

            fprintf(stdout, RESET);
        }

        printf("\n");
    }
}
