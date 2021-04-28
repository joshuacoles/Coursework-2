#ifndef COURSEWORK_2_C_HELPERS_H
#define COURSEWORK_2_C_HELPERS_H

#include <stdbool.h>
#include "grid.h"

#define RED   "\x1B[31m"
#define MAG   "\x1B[35m"
#define RESET "\x1B[0m"

/**
 * PosList, following standard practice, is comprised of the following.
 *
 * - `data`, a pointer to the backing memory allocation, of at least `sizeof(Pos) * capacity`.
 * - `length`, the current number of elements the list.
 * - `capacity`, maximum number of elements that can fit in the memory allocation.
 */
typedef struct PosList {
    Pos* data;
    int length;
    int capacity;
} PosList;

PosList allocPosList(int capacity);
void freePosList(PosList list);

void printPosList(char *prefix, PosList const *list);

void appendToPosList(PosList* list, Pos pos);

bool containsPos(PosList const *list, Pos r);
bool randomBool(double p);
int randomUniform(int r0, int r1);

void removeElement(Pos *array, int index, int *len);

int max(int a, int b);

#endif // COURSEWORK_2_C_HELPERS_H
