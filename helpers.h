#include <stdbool.h>
#include "grid.h"

bool containsGen(void const *list, int len, void* item, bool (*eq)(void*,void*));

bool contains(int const *list, int len, int r);
bool containsPos(Pos const *list, int len, Pos r);
bool randomBool(double p);
int randomUniform(int r0, int r1);
