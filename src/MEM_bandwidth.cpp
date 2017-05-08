#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rdtsc.h"
#include "utilities.h"
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <cstring>
using namespace std;
#define LOOP_TIMES 10

double getMemoryBindwidth() {
    //long arrayLength = 1 << 28;//1024mb
    long arrayLength = 1 << 28;//64mb
    int* intArray = (int*)malloc(arrayLength*sizeof(int));
    memset(intArray, 0, arrayLength*sizeof(int));
    int unrollLength = arrayLength / 8;
    uint64_t start, end, sum = 0;
    int tmp;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        for (int j=0; j<unrollLength; ++j) {
            tmp = intArray[j];
            tmp = intArray[j+unrollLength];
            tmp = intArray[j+unrollLength*2];
            tmp = intArray[j+unrollLength*3];
            tmp = intArray[j+unrollLength*4];
            tmp = intArray[j+unrollLength*5];
            tmp = intArray[j+unrollLength*6];
            tmp = intArray[j+unrollLength*7];
        }
        end = rdtsc_end();
        sum += end - start;
    	printf("RAM read bandwidth = %f MB/s\n", 1024/((double)(end - start) / 1.8e9));
    }
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        for (int j=0; j<unrollLength; ++j) {
            intArray[j] = 1;
            intArray[j+unrollLength] = 1;
            intArray[j+unrollLength*2] = 1;
            intArray[j+unrollLength*3] = 1;
            intArray[j+unrollLength*4] = 1;
            intArray[j+unrollLength*5] = 1;
            intArray[j+unrollLength*6] = 1;
            intArray[j+unrollLength*7] = 1;
        }
        end = rdtsc_end();
        sum += end - start;
    	printf("RAM write bandwidth = %f MB/s\n", 1024/((double)(end - start) / 1.8e9));
    }
    free(intArray);
}

double getMemoryBindwidth2() {
    //long arrayLength = 1 << 28;//1024mb
    long arrayLength = 1 << 28;//64mb
    int* intArray = (int*)malloc(arrayLength*sizeof(int));
    memset(intArray, 0, arrayLength*sizeof(int));
    int unrollLength = arrayLength / 16;
    uint64_t start, end, sum = 0;
    int tmp;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        for (int j=0; j<unrollLength; ++j) {
            tmp = intArray[j];
            tmp = intArray[j+unrollLength];
            tmp = intArray[j+unrollLength*2];
            tmp = intArray[j+unrollLength*3];
            tmp = intArray[j+unrollLength*4];
            tmp = intArray[j+unrollLength*5];
            tmp = intArray[j+unrollLength*6];
            tmp = intArray[j+unrollLength*7];
            tmp = intArray[j+unrollLength*8];
            tmp = intArray[j+unrollLength*9];
            tmp = intArray[j+unrollLength*10];
            tmp = intArray[j+unrollLength*11];
            tmp = intArray[j+unrollLength*12];
            tmp = intArray[j+unrollLength*13];
            tmp = intArray[j+unrollLength*14];
            tmp = intArray[j+unrollLength*15];
        }
        end = rdtsc_end();
        sum += end - start;
    	printf("RAM read bandwidth = %f MB/s\n", 1024/((double)(end - start) / 1.8e9));
    }
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        for (int j=0; j<unrollLength; ++j) {
            intArray[j] = 1;
            intArray[j+unrollLength] = 1;
            intArray[j+unrollLength*2] = 1;
            intArray[j+unrollLength*3] = 1;
            intArray[j+unrollLength*4] = 1;
            intArray[j+unrollLength*5] = 1;
            intArray[j+unrollLength*6] = 1;
            intArray[j+unrollLength*7] = 1;
            intArray[j+unrollLength*8] = 1;
            intArray[j+unrollLength*9] = 1;
            intArray[j+unrollLength*10] = 1;
            intArray[j+unrollLength*11] = 1;
            intArray[j+unrollLength*12] = 1;
            intArray[j+unrollLength*13] = 1;
            intArray[j+unrollLength*14] = 1;
            intArray[j+unrollLength*15] = 1;
        }
        end = rdtsc_end();
        sum += end - start;
    	printf("RAM write bandwidth = %f MB/s\n", 1024/((double)(end - start) / 1.8e9));
    }
    free(intArray);
}

double getMemoryBindwidth3() {
    //long arrayLength = 1 << 28;//1024mb
    long arrayLength = 1 << 28;//64mb
    int* intArray = (int*)malloc(arrayLength*sizeof(int));
    memset(intArray, 0, arrayLength*sizeof(int));
    int unrollLength = arrayLength / 4;
    uint64_t start, end, sum = 0;
    int tmp;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        for (int j=0; j<unrollLength; ++j) {
            tmp = intArray[j];
            tmp = intArray[j+unrollLength];
            tmp = intArray[j+unrollLength*2];
            tmp = intArray[j+unrollLength*3];
        }
        end = rdtsc_end();
        sum += end - start;
    	printf("RAM read bandwidth = %f MB/s\n", 1024/((double)(end - start) / 1.8e9));
    }
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        for (int j=0; j<unrollLength; ++j) {
            intArray[j] = 1;
            intArray[j+unrollLength] = 1;
            intArray[j+unrollLength*2] = 1;
            intArray[j+unrollLength*3] = 1;
        }
        end = rdtsc_end();
        sum += end - start;
    	printf("RAM write bandwidth = %f MB/s\n", 1024/((double)(end - start) / 1.8e9));
    }
    free(intArray);
}

int main() {
    cout << sizeof(int) << " " << sizeof(long) << endl;
    getMemoryBindwidth();
    getMemoryBindwidth2();
    getMemoryBindwidth2();
    return 0;
}
