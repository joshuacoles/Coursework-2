#include <stdlib.h>
#include <time.h>
#include <printf.h>

#include "grid.h"
#include "cluster_finder.h"

typedef struct GridData {
    int x_dim;
    int y_dim;
    int nConductors;
    double pSuper;
} GridData;

#define READ(str, fmt, pointer, condition) while (true) { printf(str); fscanf(stdin, fmt, pointer); if (condition) break; }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
GridData acceptInput(bool superConductors) {
    int x_dim = 10;
    int y_dim = 10;
    int n_conductors = 3;
    double pSuper = 0;

    READ("X Dimension: ", "%d", &x_dim, x_dim > 0)
    READ("Y Dimension: ", "%d", &y_dim, y_dim > 0)
    READ("Number of Conductors: ", "%d", &n_conductors, 0 <= n_conductors && n_conductors <= x_dim * y_dim);

    if (superConductors) {
        double percentage = 0;
        READ("Superconductor Percentage: ", "%lf", &percentage, 0 <= percentage && percentage <= 1);
        pSuper = percentage / 100;
    }

    return (GridData) {
            .x_dim = x_dim,
            .y_dim = y_dim,
            .nConductors = n_conductors,
            .pSuper = pSuper
    };
}
#pragma clang diagnostic pop

void q1Print(GridData data, int n_grids) {
    Grid grid = allocateGrid2D(data.x_dim, data.y_dim);

    for (int i = 0; i <= n_grids; ++i) {
        // Fill completely regenerates grid so we can reuse the allocation
        fillGrid(grid, data.nConductors, data.pSuper);
        printGrid(grid);
        printf("\n\n");
    }

    freeGrid(grid);
}

void q1a() {
    GridData data = acceptInput(false);
    q1Print(data, 3);
}

void q1b() {
    GridData data = acceptInput(true);
    q1Print(data, 3);
}

void q2a() {
    GridData data = acceptInput(false);
    data.pSuper = 0.1; // As per brief.

    Grid grid = allocateGrid2D(data.x_dim, data.y_dim);

    for (int i = 0; i < 3; ++i) {
        // Fill completely regenerates grid (including insulators) so we can reuse the allocation
        fillGrid(grid, data.nConductors, data.pSuper);

        ClusterFinder cf = newClusterFinder(&grid);
        performSearch(&cf);
        printCluster(&cf);
        printf("\n\n");
        freeClusterFinder(cf);
    }

    freeGrid(grid);
}

int queryPathsFound(GridData data) {
    Grid grid = allocateGrid2D(data.x_dim, data.y_dim);

    int pathFound = 0;

    for (int i = 0; i < 100; ++i) {
        // Fill completely regenerates grid (including insulators) so we can reuse the allocation
        fillGrid(grid, data.nConductors, data.pSuper);

        ClusterFinder cf = newClusterFinder(&grid);
        performSearch(&cf);
        pathFound += didFormPath(&cf);
        freeClusterFinder(cf);
    }


    freeGrid(grid);

    return pathFound;
}

void q2b() {
    GridData data = (GridData) {
            .x_dim = 6,
            .y_dim = 4,
            .nConductors = 10,
            .pSuper = 0.1
    };

    printf("Found %d", queryPathsFound(data));
}

void q2c() {
//    FILE* file = fopen("/Users/joshuacoles/Developer/checkouts/jc3091/cw-2-da/outNI3D.csv", "a");

    GridData data = (GridData) {
            .x_dim = 25,
            .y_dim = 25,
            .nConductors = 0,
            .pSuper = 0.1//.1
    };

    for (int nC = 100; nC <= 25 * 25; nC++) {
        data.nConductors = nC;
//        for (int pD = 1; pD <= 10; pD++) {
//        for (int pD = 0; pD <= 0; pD++) {
//            data.pSuper = pD / (double) 10;
            fprintf(stdout, "%d, %d, %d\n", nC, 10, queryPathsFound(data));
//        }

//        fflush(file);
    }

//    fclose(file);
}

void qq() {
    GridData data = (GridData) {
            .x_dim = 25,
            .y_dim = 25,
            .nConductors = 400,
            .pSuper = 0.1//.1
    };

    Grid grid = allocateGrid2D(data.x_dim, data.y_dim);

    for (int i = 0; i < 1; ++i) {
        // Fill completely regenerates grid (including insulators) so we can reuse the allocation
        fillGrid(grid, data.nConductors, data.pSuper);

        printGrid(grid);

        ClusterFinder cf = newClusterFinder(&grid);
        performSearch(&cf);
        printCluster(&cf);
        printf("\n\n");
        freeClusterFinder(cf);
    }

    freeGrid(grid);
}

void errSweep() {
    for (int x = 1; x <= 100; x++) {
        for (int y = 1; y <= 100; y++) {
            Grid grid = allocateGrid2D(x, y);

            for (int n_c = 0; n_c <= x * y; n_c++) {
                for (int pD = 0; pD <= 10; pD++) {

                    for (int gridIndex = 0; gridIndex < 100; ++gridIndex) {
                        // Fill completely regenerates grid (including insulators) so we can reuse the allocation
                        fillGrid(grid, n_c, pD / (double) 10);

                        ClusterFinder cf = newClusterFinder(&grid);
                        performSearch(&cf);
//                        printCluster(&cf);
//                        printf("\n\n");
                        freeClusterFinder(cf);
                    }

                }
            }

            freeGrid(grid);
        }
    }
}

int main() {
//    srand(11);
    srand(time(NULL));

    errSweep();

    return 0;
}
