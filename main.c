#include <printf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "entrypoints/questions.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    int choice;

    printf("Please select one of the following options: \n");
    printf("\tq1a: 1\n");
    printf("\tq1b: 2\n");
    printf("\tq2a: 3\n");
    printf("\tq2b: 4\n");
    printf("\tq2c: 5\n");
    printf("\t q3: 6\n");

    READ("Option: ", "%d", &choice, 0 < choice && choice <= 6);

    switch (choice) {
        case 1:
            q1a();
            break;

        case 2:
            q1b();
            break;

        case 3:
            q2a();
            break;

        case 4:
            q2b();
            break;

        case 5:
            q2c();
            break;

        case 6:
            q3();
            break;

        default:
            fprintf(stderr, "Invalid choice");
            exit(1);
    }

    return 0;
}
