#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rdtsc.h"
#include <fstream>
#include <ctime>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "utilities.h"
#include <pthread.h>
#include <math.h>

char* fileNames[]={"/Users/andy/Documents/Spring17/221/CSE221/file1","/Users/andy/Documents/Spring17/221/CSE221/file2",
                   "/Users/andy/Documents/Spring17/221/CSE221/file3","/Users/andy/Documents/Spring17/221/CSE221/file4",
                   "/Users/andy/Documents/Spring17/221/CSE221/file5","/Users/andy/Documents/Spring17/221/CSE221/file6",
                   "/Users/andy/Documents/Spring17/221/CSE221/file7","/Users/andy/Documents/Spring17/221/CSE221/file8"};
long arraySize = 1 << 26;//64mb
void createFile() {
    char *writeChar = (char*)malloc(arraySize*sizeof(char));
    memset(writeChar, 'A', arraySize*sizeof(char));
    long f;
    for (int i=0;i<8;i++) {
        f = open(fileNames[i], O_WRONLY | O_CREAT);
        write(f, writeChar, arraySize);
        close(f);
    }
}

size_t block_size = 4 * 1024;

void read_sequential(char* filename, size_t file_size) {

    int fd = open(filename, O_SYNC | O_RDONLY);
    if (!fd) {
        exit(1);
    }

    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
        exit(1);
    }

    char* buffer = (char*)malloc(block_size);
    double sum = 0;
    long long int cur_size = 0;

    cur_size = 0;
    uint64_t start, end;
    int num_blocks = 0;
    while (true) {
        start = rdtsc_start();
        read(fd, buffer, block_size);
        end = rdtsc_end();
        sum += (end - start) / 2.6 / 1000;
        num_blocks += 1;
        cur_size += block_size;
        if (cur_size > file_size) break;
    }
    printf("number of blocks is %d\n", num_blocks);
    printf("total us is %f\n", sum);
    printf("average is %f\n", sum/num_blocks);
    delete buffer;
}

void read_random(char* filename, size_t file_size) {
    int fd = open(filename, O_SYNC | O_RDONLY);
    if (!fd) {
        exit(1);
    }

    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
        exit(1);
    }

    char* buffer = (char*)malloc((size_t)block_size);

    double sum = 0;
    std::vector<long long int> access_list;
    size_t vector_length = file_size / block_size;
    for (long long int i=0; i<vector_length; ++i) {
        access_list.push_back(i);
    }
    std::random_shuffle(access_list.begin(), access_list.end());

    uint64_t start, end;
    int num_blocks = 0;

    for (long long int i=0; i<vector_length; ++i) {
        start = rdtsc_start();
        lseek(fd, block_size*access_list[i], SEEK_SET);
        read(fd, buffer, block_size);
        end = rdtsc_end();
        sum += (end - start) / 2.6 / 1000;
        num_blocks += 1;
    }
    printf("number of blocks is %d\n", num_blocks);
    printf("total clock cycle is %f\n", sum);
    printf("average is %f\n", sum/num_blocks);
    delete buffer;
}


int main() {
    //createFile();
    srand((unsigned int)time(NULL));
    pid_t pids[8];
    printf("\n");
    int numThreads = 8;
    for (int i=0; i<numThreads; ++i) {
        if ((pids[i] = fork()) < 0) {
            perror("fork");
            abort();
        }
        else if (pids[i] == 0) {
            //read_sequential(fileNames[i], arraySize);
            read_random(fileNames[i], arraySize);
            exit(0);
        }
        else {
            printf("main thread\n");
        }
    }
    sleep(5);
    printf("all done!\n");
    return 0;
}
