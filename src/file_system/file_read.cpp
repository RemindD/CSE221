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

void read_sequential(int fd, size_t file_size) {

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
        sum += (end - start) / 1000 / 2.6;
        num_blocks += 1;
        lseek(fd, (off_t)block_size, SEEK_CUR);
        cur_size += block_size;
        if (cur_size > file_size) break;

    }
    printf("number of blocks is %d\n", num_blocks);
    printf("total us is %f\n", sum);
    printf("average is %f\n", sum/num_blocks);
}

void read_random(int fd, size_t file_size) {

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
        lseek(fd, block_size*access_list[i], SEEK_SET);
        start = rdtsc_start();
        read(fd, buffer, block_size);
        end = rdtsc_end();
        sum += (end - start) / 1000 / 2.6;
        num_blocks += 1;
    }
    printf("number of blocks is %d\n", num_blocks);
    printf("total clock cycle is %f\n", sum);
    printf("average is %f\n", sum/num_blocks);
}
int main(int argc, char** argv) {
    char *filename = (char*)"/Volumes/Macintosh HD/Users/richard/Documents/graduate_study/3rd_semester/CSE255/git-repo/Data/users-partition.pickle";
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

    long long int file_size = fs.tellg(), cur_size = 0;
    printf("File size is %lld\n", file_size);
    fs.close();

    int fd = open(filename, O_RDONLY);
    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
    }
    read_sequential(fd, file_size);
    read_random(fd, file_size);
}