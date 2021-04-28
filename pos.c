#include "pos.h"

Pos offsetPosition(Pos a, int dx, int dy, int dz) {
    return (Pos) {
            .x = a.x + dx,
            .y = a.y + dy,
            .z = a.z + dz,
    };
}

bool posEq(Pos a, Pos b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
