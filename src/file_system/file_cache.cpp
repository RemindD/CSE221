//
// Created by Xingdong Li on 6/3/17.
//

#include "rdtsc.h"
#include <stdlib.h>
#include <fstream>

int block_size = 4 * 1024;

int main(int argc, char **argv) {
    std::ifstream fs;
    char *filename;
    if (argc == 2) {
        filename = argv[1];
    }
    else {
        printf("please enter filename\n");
        exit(1);
    }
    fs.open(filename, std::ios::binary | std::ios::ate);

    if (!fs) {
        printf("file open error\n");
        exit(1);
    }

    long long int file_size = fs.tellg(), cur_size = 0;
    printf("File size is %lld\n", file_size);
    fs.close();

    char* buffer = (char*)malloc((size_t)block_size);
    long long int sum = 0;

    fs.open(filename, std::ifstream::in);
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
    return 0;

}