#ifndef COURSEWORK_2_C_QUESTIONS_H
#define COURSEWORK_2_C_QUESTIONS_H

typedef struct GridData {
    int x_dim;
    int y_dim;
    int nConductors;
    double pSuper;
} GridData;

// Macro to take input until condition is met.
#define READ(str, fmt, pointer, condition) while (true) { printf(str); fscanf(stdin, fmt, pointer); if (condition) break; }

void q1a();
void q1b();

void q2a();
void q2b();
void q2c();

void dataCollection();

#endif //COURSEWORK_2_C_QUESTIONS_H
