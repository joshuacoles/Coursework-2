#include <printf.h>
#include <assert.h>
#include "grid.h"
#include "helpers.h"
#include "cluster_finder.h"

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint) {
    Pos *cluster = malloc(grid->x_dim * grid->y_dim * sizeof(Pos));
    Pos *nextToProcess = malloc(grid->x_dim * grid->y_dim * sizeof(Pos));

    cluster[0] = initialPoint;
    nextToProcess[0] = initialPoint;

    return (ClusterFinder) {
            .grid = grid,
            .initialPoint = initialPoint,
            .cluster = cluster,
            .clusterLength = 1,
            .nextToProcess = nextToProcess,
            .nextToProcessLength = 1,

            // Avoid reallocation
            .found = malloc(grid->x_dim * grid->y_dim * sizeof(Pos)),
            .foundLength = 0,
    };
}

ClusterFinder newClusterFinder(Grid *grid) {
    int x = randomUniform(0, grid->x_dim);
    int y = randomUniform(0, grid->y_dim);

    return newClusterFinderWithInitialPoint(grid, (Pos) {x, y});
}

void printPosList(char* prefix, Pos* list, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%s(%d, %d)\n", prefix, list[i].x, list[i].y);
    }
}

const int MAX_REACHABLE_SIZE = 8;

void performSearchStep(ClusterFinder *self) {
    printf("\tPerforming step\n");
    printf("\t\tNTP:\n");
    printPosList("\t\t\t", self->nextToProcess, self->nextToProcessLength);

    Pos *reachableCache = malloc(sizeof(Pos) * MAX_REACHABLE_SIZE);
    for (int i = 0; i < self->nextToProcessLength; ++i) {
        Pos pos = self->nextToProcess[i];
        int reachableFrom = directlyReachableFrom(*self->grid, pos, reachableCache);

        for (int j = 0; j < reachableFrom; ++j) {
            if (!containsPos(self->cluster, self->clusterLength, reachableCache[j])) {
                // TOdo make macro or explain ++ logic
                self->cluster[self->clusterLength++] = reachableCache[j];
                self->found[self->foundLength++] = reachableCache[j];
            }
        }

        printf("\t\t%d == %d\n", self->foundLength, reachableFrom);
    }

    printf("\t\tFound:\n");
    printPosList("\t\t\t", self->found, self->foundLength);

    // Swap arrays
    Pos* mv = self->nextToProcess;
    self->nextToProcess = self->found;
    self->found = mv;
    self->nextToProcessLength = self->foundLength;
    self->foundLength = 0;
}

void performSearch(ClusterFinder *self) {
    printf("Starting search\n");
    while (self->nextToProcessLength != 0) {
        performSearchStep(self);
    }
    printf("Ending search\n");
}

bool didFormPath(ClusterFinder *self) {
    bool connectedTop = false;
    bool connectedBottom = false;

    for (int i = 0; i < self->clusterLength; ++i) {
        Pos pos = self->cluster[i];
        if (pos.y == 0) connectedBottom = true;
        if (pos.y == self->grid->y_dim) connectedTop = true;
    }

    return connectedBottom && connectedTop;
}
