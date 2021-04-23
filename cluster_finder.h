#ifndef COURSEWORK_2_C_CLUSTER_FINDER_H
#define COURSEWORK_2_C_CLUSTER_FINDER_H

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
    Pos *found;
    int foundLength;
} ClusterFinder;

ClusterFinder newClusterFinderWithInitialPoint(Grid *grid, Pos initialPoint);
ClusterFinder newClusterFinder(Grid *grid);

const int MAX_REACHABLE_SIZE;

void performSearchStep(ClusterFinder *self);
void performSearch(ClusterFinder *self);
bool didFormPath(ClusterFinder *self);

#endif //COURSEWORK_2_C_CLUSTER_FINDER_H
