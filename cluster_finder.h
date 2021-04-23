#ifndef COURSEWORK_2_C_CLUSTER_FINDER_H
#define COURSEWORK_2_C_CLUSTER_FINDER_H

#include "grid.h"
#include "helpers.h"

typedef struct ClusterFinder {
    Grid *grid;
    Pos initialPoint;

    PosList cluster;

    // The leading edge of the cluster, hence a subset of `cluster`
    PosList nextToProcess;
    PosList found;
} ClusterFinder;

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint);
ClusterFinder newClusterFinder(Grid *grid);
void freeClusterFinder(ClusterFinder clusterFinder);

const int MAX_REACHABLE_SIZE;

void performSearchStep(ClusterFinder *self);
void performSearch(ClusterFinder *self);
void printCluster(ClusterFinder *self);
bool didFormPath(ClusterFinder *self);

#endif //COURSEWORK_2_C_CLUSTER_FINDER_H
