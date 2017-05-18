//
// Created by Xingdong Li on 5/7/17.
//

#include <stdlib.h>
#include <stdio.h>
#include "rdtsc.h"
#include "utilities.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <ctime>
using namespace std;
#define LOOP_TIMES 10
#define LARGE_LOOP_TIMES 1000

double getMemoryAccessTime(long size, long stride) {
    long array_size = size / 4;
    long array_stride = stride / 4;
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

void getMemoryBindwidth() {
    long arrayLength = 1 << 24;//16mb*64=1024mb
    int* intArray = (int*)malloc(arrayLength*sizeof(int));
    memset(intArray, -1, arrayLength*sizeof(int));
    long unrollLength = arrayLength / 8;
    uint64_t start, end;
    int tmp;
    int blockNumber = 8;
    long blockLength = arrayLength / blockNumber;
    for (int l=0; l<LOOP_TIMES; ++l) {
        start = rdtsc_start();
        for (int i=0; i<32; ++i) {
            for (long j=0;j<blockLength;j+=32) {//avoid cache
                tmp = intArray[i+j];
                tmp = intArray[i+j+blockLength];
                tmp = intArray[i+j+blockLength*2];
                tmp = intArray[i+j+blockLength*3];
                tmp = intArray[i+j+blockLength*4];
                tmp = intArray[i+j+blockLength*5];
                tmp = intArray[i+j+blockLength*6];
                tmp = intArray[i+j+blockLength*7];
            }
        }
        end = rdtsc_end();
        printf("RAM read overhead = %ld\n", end - start);
    }
    for (int l=0; l<LOOP_TIMES; ++l) {
        start = rdtsc_start();
        for (int i=0; i<32; ++i) {
            for (long j=0;j<blockLength;j+=32) {//avoid cache
                intArray[i+j] = 1;
                intArray[i+j+blockLength] = 1;
                intArray[i+j+blockLength*2] = 1;
                intArray[i+j+blockLength*3] = 1;
                intArray[i+j+blockLength*4] = 1;
                intArray[i+j+blockLength*5] = 1;
                intArray[i+j+blockLength*6] = 1;
                intArray[i+j+blockLength*7] = 1;
            }
        }
        end = rdtsc_end();
        printf("RAM write overhead = %ld\n", end - start);
    }
    free(intArray);
}

void createFile() {
    srand(time(NULL));
    long arraySize = 1 << 26;//64mb
    char *writeChar = (char*)malloc(arraySize*sizeof(char));
    memset(writeChar, 'A', arraySize*sizeof(char));
    long f;
    f = open("testFile", O_WRONLY | O_CREAT);
    write(f, writeChar, arraySize);
    close(f);
}

void getPageFaultTime() {
    long arraySize = 1 << 26;//64mb
    char* data = NULL;
    long f;
    uint64_t start, end;
    f = open("testFile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (f == -1) {
        printf("File Open Failed\n");
        return;
    }
    char* m = (char*)mmap(NULL, arraySize, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (m == MAP_FAILED) {
        printf("MMAP Failed\n");
        return;
    }
    close(f);
    rdtsc_start();
    start = rdtsc_start();
    for (int i=0; i<16384; ++i) {
        m[i*4096] = 'A';
    }
    end = rdtsc_end();
    munmap(m, arraySize);
    printf("Page Fault Overhead = %ld\n", end - start);
}

int main() {
    long stride[6] = {4, 16, 64, 256, 1024, 4096};
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
    getMemoryBindwidth();
    createFile();
    getPageFaultTime();
    return 0;
}
