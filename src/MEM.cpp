//
// Created by Xingdong Li on 5/7/17.
//

#include <stdlib.h>
#include <stdio.h>
#include "rdtsc.h"
#define LARGE_LOOP_TIMES 100

double getMemoryAccessTime(long size, long stride) {
    long array_size = size / 8;
    long array_stride = stride / 8;
    long* data = (long*)malloc(array_size * sizeof(long));
    for (long i=0; i<array_size; ++i) {
        data[i] = (i+array_stride) % array_size;
    }
    uint64_t start, end;
    long index = 0;
    start = rdtsc_start();
    for (long i=0; i<LARGE_LOOP_TIMES; ++i) {
        index = data[index];
    }
    end = rdtsc_end();
    double sum = (double)(end-start);
    free(data);
    return sum / LARGE_LOOP_TIMES;
}


int main() {
    long stride[6] = {4, 16, 64, 256, 1028, 4096};
    long size[20];
    size[0] = 4096;
    for (int i=0; i<16; ++i) {
        size[i+1] = size[i] * 2;
    }
    double *memoryAccessTime = new double [20];
    for (int j=0; j<6; ++j) {
        for (int i = 0; i < 16; ++i) {
            memoryAccessTime[i] = getMemoryAccessTime(size[i], stride[j]);
            printf("%f\n", memoryAccessTime[i]);
        }
        printf("------------------------------\n");
    }


}