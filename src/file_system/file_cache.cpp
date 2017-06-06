//
// Created by Xingdong Li on 6/3/17.
//

#include "rdtsc.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

size_t block_size = 4 * 1024;

int read_cache(char *filename, long long int file_size) {
    int fs = open(filename, O_RDONLY);
    long long int cur_size;

    char* buffer = (char*)malloc((size_t)block_size);
    double sum = 0;

    cur_size = 0;
    while (true) {
        read(fs, buffer, block_size);
        lseek(fs, block_size, SEEK_CUR);
        cur_size += block_size;
        if (cur_size > file_size) break;
    }
    lseek(fs, 0, SEEK_SET);
    uint64_t start, end;
    int num_blocks = 0;
    cur_size = 0;
    while (true) {
        start = rdtsc_start();
        read(fs, buffer, block_size);
        end = rdtsc_end();
        sum += (end - start) / 1000 / 2.6;
        num_blocks += 1;
        lseek(fs, block_size, SEEK_CUR);
        cur_size += block_size;
        if (cur_size > file_size) break;
    }
    close(fs);
    printf("number of blocks is %d\n", num_blocks);
    printf("total us is %f\n", sum);
    printf("average is %f\n", sum/num_blocks);
    return 0;
}

int main(int argc, char** argv) {
    char* filename;
    if (argc == 2) {
        filename = argv[1];
    }
    else {
        printf("no filename");
        exit(1);
    }

    printf("%s\n", filename);
    std::ifstream fs;
    fs.open(filename, std::ios::binary | std::ios::ate);

    if (!fs) {
        printf("file open error\n");
        exit(1);
    }

    long long int file_size = fs.tellg(), cur_size = 0;
    printf("File size is %lld\n", file_size);
    fs.close();
    long long int gb = 1024 * 1024;
    long long int file_sizes[6] = {100*gb, 1000*gb, 2000*gb, 3000*gb, 4000*gb, 5000*gb};
    for (int i=0; i<6; ++i) read_cache(filename, file_sizes[i]);
}