#include <printf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "entrypoints/questions.h"
#include "entrypoints/data_collection.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    dataCollection2D();

    return 0;
}
