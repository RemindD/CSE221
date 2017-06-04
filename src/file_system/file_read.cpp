//
// Created by Xingdong Li on 6/3/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rdtsc.h"
#include <fstream>

int block_size = 4 * 1024;

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
    std::random_shuffle(access_list.begin(), access_list.end());

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
int main(int argc, char** argv) {
    char *filename;
    if (argc == 2) {
        filename = argv[1];
    }
    else {
        printf("please enter filename\n");
        exit(1);
    }
    read_sequential(filename);
    read_random(filename);
}