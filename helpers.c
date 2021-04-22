#include <stdbool.h>
#include <stdlib.h>

bool contains(int const *l, int len, int r) {
    for (int i = 0; i < len; ++i) {
        if (l[i] == r) return true;
    }

    return false;
}

// Todo fix this
int randomUniform(int r0, int r1) {
    return (r0 + r1) / (double) 2;
}

bool randomBool(double p) {
    return (rand() / (double) RAND_MAX) < p;
}
