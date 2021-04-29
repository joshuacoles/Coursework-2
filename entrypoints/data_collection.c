#include "../cluster_finder.h"
#include "../grid.h"
#include <printf.h>
#include <time.h>
#include <libc.h>
#include <stdlib.h>
#include "questions.h"
#include "data_collection.h"

/*
 * Data collection for stats
 * */

void dataCollection2D() {
    int xDim = 12;
    int yDim = 12;

    int minNC = 0;
    int macNC = xDim * yDim;
    int ncStep = 1;

    Grid grid = allocateGrid2D(xDim, yDim);

    char path[100];
    sprintf(path, "./stats/dc-%d-%d-%d-%d.csv", xDim, yDim, minNC, macNC);
    FILE *file = fopen(path, "a");

    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long startTime = 1000000 * tv.tv_sec + tv.tv_usec;

    for (int nC = minNC; nC <= macNC; nC += ncStep) {
        for (int pD = 0; pD <= 10; ++pD) {
            int pathsFound = 0;

            for (int i = 0; i < 100; ++i) {
                // Fill completely regenerates grid (including insulators) so we can reuse the allocation
                fillGrid(grid, nC, pD / (double) 10);

                ClusterFinder cf = newClusterFinder(&grid);
                performSearch(&cf);
                pathsFound += didFormPath(&cf);
                freeClusterFinder(cf);
            }

            gettimeofday(&tv, NULL);
            unsigned long time = (1000000 * tv.tv_sec + tv.tv_usec) - startTime;
            fprintf(file, "%lu,%d,%d,%d\n", time, nC, pD, pathsFound);
            fprintf(stdout, "%lu,%d,%d,%d\n", time, nC, pD, pathsFound);
        }

        fflush(file);
    }

    freeGrid(grid);
}