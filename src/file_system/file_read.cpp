//
// Created by Xingdong Li on 6/3/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "rdtsc.h"

size_t block_size = 4 * 1024;

void read_sequential(char* filename, size_t file_size) {

    int fd = open(filename, O_RDONLY);
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
    int fd = open(filename, O_RDONLY);
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
        lseek(fd, (off_t)block_size*access_list[i], SEEK_SET);
        read(fd, buffer, block_size);
        end = rdtsc_end();
        sum += (end - start) / 2.6 / 1000;
        num_blocks += 1;
    }
    printf("number of blocks is %d\n", num_blocks);
    printf("total us is %f\n", sum);
    printf("average is %f\n", sum/num_blocks);
    delete buffer;
}

int main(int argc, char** argv) {
    char *filename;
    if (argc == 2) {
        filename = argv[1];
    }
    else {
        printf("no file name");
        exit(1);
    }
    /*
     * accessing file on local disk need file path
     * accessing remote file need to use the file path with /Volumes/ prefix
     */
    std::ifstream fs;
    fs.open(filename, std::ios::binary | std::ios::ate);
    if (!fs) {
        printf("file open error\n");
        exit(1);
    }

    size_t file_size = 1024 * 1024 * 10;

    std::vector<size_t> file_sizes;

    read_sequential(filename, file_size);
    read_random(filename, file_size);


}