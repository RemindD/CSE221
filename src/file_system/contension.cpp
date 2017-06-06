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

int block_size = 4 * 1024;
char* fileNames[]={"file1","file2","file3","file4","file5","file6","file7","file8"};

void createFile() {
    srand(time(NULL));
    long arraySize = 1 << 26;//64mb
    char *writeChar = (char*)malloc(arraySize*sizeof(char));
    memset(writeChar, 'A', arraySize*sizeof(char));
    long f;
    for (int i=0;i<8;i++) {
        f = open(fileNames[i], O_WRONLY | O_CREAT);
        write(f, writeChar, arraySize);
        close(f);
    }
}

void read_sequential(char* filename) {
    std::ifstream fs;

    fs.open(filename, std::ios::binary | std::ios::ate);

    if (!fs) {
        printf("file open error\n");
        exit(1);
    }

    fs.rdbuf()->pubsetbuf(0, 0);
    char* buffer = (char*)malloc((size_t)block_size);
    long long int file_size = fs.tellg(), cur_size = 0;
    printf("File size is %lld\n", file_size);
    while (true) {
        fs.read(buffer, block_size);
        fs.seekg(block_size, std::ios::cur);
        cur_size += block_size;
        if (cur_size > file_size) break;
    }
    fs.close();


    long long int sum = 0;

    fs.open(filename, std::ifstream::in);
    cur_size = 0;
    uint64_t start, end;
    int num_blocks = 0;
    while (true) {
        start = rdtsc_start();
        fs.read(buffer, block_size);
        end = rdtsc_end();
        sum += (end - start);
        num_blocks += 1;
        fs.seekg(block_size, std::ios::cur);
        cur_size += block_size;
        if (cur_size > file_size) break;

    }
    fs.close();
    printf("number of blocks is %d\n", num_blocks);
    printf("total clock cycle is %lld\n", sum);
    printf("average is %f\n", (double)sum/num_blocks);
}

void read_random(char* filename) {
    std::ifstream fs;

    fs.open(filename, std::ios::binary | std::ios::ate);

    if (!fs) {
        printf("file open error\n");
        exit(1);
    }

    fs.rdbuf()->pubsetbuf(0, 0);
    char* buffer = (char*)malloc((size_t)block_size);

    long long int file_size = fs.tellg(), cur_size = 0;
    printf("File size is %lld\n", file_size);
    while (true) {
        fs.read(buffer, block_size);
        fs.seekg(block_size, std::ios::cur);
        cur_size += block_size;
        if (cur_size > file_size) break;
    }
    fs.close();

    std::vector<long long int> access_list;
    long long int vector_length = file_size / block_size;
    for (long long int i=0; i<vector_length; ++i) {
        access_list.push_back(i);
    }
    //std::random_shuffle(access_list.begin(), access_list.end());

    long long int sum = 0;

    fs.open(filename, std::ifstream::in);
    uint64_t start, end;
    int num_blocks = 0;

    for (long long int i=0; i<vector_length; ++i) {
        fs.seekg(block_size*access_list[i], std::ios::beg);
        start = rdtsc_start();
        fs.read(buffer, block_size);
        end = rdtsc_end();
        sum += (end - start);
        num_blocks += 1;
    }
    fs.close();
    printf("number of blocks is %d\n", num_blocks);
    printf("total clock cycle is %lld\n", sum);
    printf("average is %f\n", (double)sum/num_blocks);
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
            read_sequential(fileNames[i]);
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
