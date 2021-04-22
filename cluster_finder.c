#include "grid.h"
#include "helpers.h"

typedef struct ClusterFinder {
    // TODo why is this a pointer?
    Grid *grid;
    Pos initialPoint;

    Pos *cluster;
    int clusterLength;

    // The leading edge of the cluster, hence a subset of `cluster`
    Pos *nextToProcess;
    int nextToProcessLength;

    // The leading edge of the cluster, hence a subset of `cluster`
    Pos *temp;
    int tempLength;
} ClusterFinder;

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint) {
    return (ClusterFinder) {
            .grid = grid,
            .initialPoint = initialPoint,
            .cluster = malloc(grid->x_dim * grid->y_dim * sizeof(Pos)),
            .clusterLength = 0,
            .nextToProcess = malloc(grid->x_dim * grid->y_dim * sizeof(Pos)),
            .nextToProcessLength = 0,

            .temp = malloc(grid->x_dim * grid->y_dim * sizeof(Pos)),
            .tempLength = 0,
    };
}

ClusterFinder newClusterFinder(Grid *grid) {
    int x = randomUniform(0, grid.x_dim);
    int y = randomUniform(0, grid.y_dim);

    return newClusterFinderWithInitialPoint(grid, (Pos) {x, y});
}

const int MAX_REACHABLE_SIZE = 8;

void performSearchStep(ClusterFinder *self) {
    Pos *reachableCache = malloc(sizeof(Pos) * MAX_REACHABLE_SIZE);
    for (int i = 0; i < self->nextToProcessLength; ++i) {
        Pos pos = self->nextToProcess[i];
        int reachableFrom = directlyReachableFrom(*self->grid, pos, reachableCache);

        for (int j = 0; j < reachableFrom; ++j) {
            if (!containsPos(self->cluster, self->clusterLength, reachableCache[j])) {
                // TOdo make macro or explain ++ logic
                self->temp[self->tempLength++] = reachableCache[j];
                self->cluster[self->clusterLength++] = reachableCache[j];
            }
        }
    }

    self->nextToProcessLength = 0;
    // Explain why we are using a temp array
    for (int i = 0; i < self->tempLength; ++i) {
        self->nextToProcess[self->nextToProcessLength++] = self->temp[i];
    }

    self->tempLength = 0;
}

void performSearch(ClusterFinder *self) {
    while (self->nextToProcess != 0) {
        performSearchStep(self);
    }
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
