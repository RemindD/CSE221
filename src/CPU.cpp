//
// Created by Xingdong Li on 4/22/17.
//


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rdtsc.h"
#include "utilities.h"

#define LOOP_TIMES 100000
#define REPEAT_TIMES 10

double getReadOverhead() {
    double sum = 0;
    uint64_t start, end;

    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        end = rdtsc();
        sum += (end - start);
    }
    return sum / (double)LOOP_TIMES;
}

double getLoopOverhead() {
    uint64_t start, end;
    start = rdtsc();
    for (int i=0; i<LOOP_TIMES; ++i) {}
    end = rdtsc();
    return (double)(end - start) / (double)LOOP_TIMES;
}

void getProcedureOverhead(std::vector<double> &overhead){
    uint64_t start, end;
    double sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure0();
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure1(0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure2(0, 0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure3(0, 0, 0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure4(0, 0, 0, 0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure5(0, 0, 0, 0, 0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure6(0, 0, 0, 0, 0, 0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc();
        procedure7(0, 0, 0, 0, 0, 0, 0);
        end = rdtsc();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);
}

int main() {
    for (int i=0; i<REPEAT_TIMES; ++i) {
        printf("read overhead for %d times is %f\n", i, getReadOverhead());
    }

    for (int i=0; i<REPEAT_TIMES; ++i) {
        printf("loop overhead for %d times is %f\n", i, getLoopOverhead());
    }

    std::vector<double> procedureOverhead;
    getProcedureOverhead(procedureOverhead);
    for (int i=0; i<8; ++i) {
        printf("The average ovehead of a function with %d integer arguments is %f\n", i, procedureOverhead[i]);
    }
    return 0;
}
