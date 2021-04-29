//
// Created by Joshua Coles on 28/04/2021.
//

#include "questions.h"

#include <stdlib.h>
#include <libc.h>
#include <time.h>
#include <printf.h>

#include "../grid.h"
#include "../cluster_finder.h"
#include "data_collection.h"

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
        READ("Superconductor Percentage (no %% sign): ", "%lf", &percentage, 0 <= percentage && percentage <= 100);
        pSuper = percentage / 100;
    }

    return (GridData) {
            .x_dim = x_dim,
            .y_dim = y_dim,
            .nConductors = n_conductors,
            .pSuper = pSuper
    };
}

/*
 * Question 1
 * */

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

/*
 * Question 2
 * */

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


void q2b() {
    GridData data = (GridData) {
            .x_dim = 6,
            .y_dim = 4,
            .nConductors = 10,
            .pSuper = 0.1
    };

    printf("Found a path in %d/100, 6x4 grids with 10 conductors and 10%% fSC\n", queryPathsFound(data));
}

void q2c() {
    GridData data = (GridData) {
            .x_dim = 100,
            .y_dim = 100,
            .nConductors = 6000,
            .pSuper = 0.1
    };

    printf("Please wait a short time...\n");

    printf("Found a path in %d/100, 100x100 grids with 6,000 conductors and 10%% fSC\n", queryPathsFound(data));
}

void q3() {
    Grid grid = allocateGrid3D(10, 10, 10);

    int pathFound = 0;

    for (int nC = 0; nC <= 1000; nC++) {
        for (int i = 0; i < 100; ++i) {
            // Fill completely regenerates grid (including insulators) so we can reuse the allocation
            fillGrid(grid, nC, 0.1);

            ClusterFinder cf = newClusterFinder(&grid);
            performSearch(&cf);
            pathFound += didFormPath(&cf);
            freeClusterFinder(cf);
        }

        if (pathFound >= 50) {
            printf("Exceeds 50%% chance of connection in 10^3 grid with fSC = 10%%, at %d condcutors\n", nC);
            break;
        }
    }

    freeGrid(grid);
}
