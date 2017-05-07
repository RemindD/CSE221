//
// Created by Xingdong Li on 4/22/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rdtsc.h"
#include "utilities.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
using namespace std;
#define LOOP_TIMES 10
#define REPEAT_TIMES 10
#define LARGE_LOOP_TIMES 100000

double getReadOverhead() {
    double sum = 0;
    uint64_t start, end;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        end = rdtsc_end();
        sum += (end - start);
    }
    return sum / (double)LOOP_TIMES;
}

double getLoopOverhead() {
    uint64_t start, end;
    start = rdtsc_start();
    for (int i=0; i<LARGE_LOOP_TIMES; ++i) {}
    end = rdtsc_end();
    return (double)(end - start) / (double)LARGE_LOOP_TIMES;
}

void getProcedureOverhead(std::vector<double> &overhead){
    uint64_t start, end;
    double sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure0();
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure1(0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure2(0, 0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure3(0, 0, 0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure4(0, 0, 0, 0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure5(0, 0, 0, 0, 0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure6(0, 0, 0, 0, 0, 0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);

    sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        procedure7(0, 0, 0, 0, 0, 0, 0);
        end = rdtsc_end();
        sum += (end - start);
    }
    overhead.push_back(sum / (double)LOOP_TIMES);
}

double getSystemCallOverhead() {
    uint64_t start, end;
    double sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        syscall(SYS_getpid);
        end = rdtsc_end();
        sum += (end - start);
    }
    return sum / (double)LOOP_TIMES;
}

void* newThread(void*) { pthread_exit(NULL); }

double getThreadCreationOverhead() {
    uint64_t start, end;
    pthread_t threadId;
    double sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        pthread_create(&threadId, NULL, newThread, NULL);
        pthread_join(threadId, NULL);
        end = rdtsc_end();
        sum += (end - start);
    }
    return sum / (double)LOOP_TIMES;
}

double getProcessCreationOverhead() {
    uint64_t start, end;
    int SUC_TIMES = 0;
    pid_t pid;
    double sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        pid = fork();
        switch (pid) {
            case -1:
                exit(-1);
            case 0:
                exit(0);
            default:
                wait(NULL);
                end = rdtsc_end();
                sum += (end - start);
                SUC_TIMES++;
        }
    }
    return sum / (double)SUC_TIMES;
}

double getReadWriteSwitch(int *myPipe1, int *myPipe2) {

    uint64_t start, end;
    int temp = 1;
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        return -1;
    }
    if (pid == 0) {
        write(myPipe1[1], &temp, sizeof(int));
        read(myPipe2[0], &temp, sizeof(int));
        exit(1);
    }
    else {
        start = rdtsc_start();
        read(myPipe1[0], &temp, sizeof(int));
        write(myPipe2[1], &temp, sizeof(int));
        end = rdtsc_end();
    }

    double result = 0.0;
    if(end > start){
        result = (double)(end - start);
    }
    return result;
}

double getReadWrite(int *myPipe) {
    uint64_t start, end;
    int temp = 1;
    start = rdtsc_start();
    write(myPipe[1], &temp, sizeof(int));
    read(myPipe[0], &temp, sizeof(int));
    end = rdtsc_end();
    return (double)(end - start);
}

double getProcessSwitchContextOverhead() {
    double sum = 0.0;
    int times = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        int myPipe0[2];
        pipe(myPipe0);
        int myPipe1[2];
        pipe(myPipe1);
        int myPipe2[2];
        pipe(myPipe2);
        double readwrite = getReadWrite(myPipe0);
        double readwriteswitch = getReadWriteSwitch(myPipe1, myPipe2);
//        printf("read write: %f\n", readwrite);
//        printf("read write switch: %f\n", readwriteswitch);
        if (readwriteswitch - readwrite > 0) {
            sum += (readwriteswitch - readwrite*2);
            ++times;
        }
    }

    return sum / times;
}

void *getRead(void *fd) {
    int temp = 1;
    write(*(int*)fd+1, &temp, sizeof(int));
}

double getThreadSwitch() {
    int myPipe[2];
    pipe(myPipe);
    int temp = 1;
    uint64_t start, end;
    pthread_t threadId;

    pthread_create(&threadId, NULL, getRead, &myPipe[0]);
    start = rdtsc_start();
    read(myPipe[0], &temp, sizeof(temp));
    end = rdtsc_end();
    pthread_join(threadId, NULL);
    return end - start;
}
double getThreadRead() {
    int myPipe[2];
    pipe(myPipe);
    int temp = 1;
    uint64_t start, end;
    write(myPipe[1], &temp, sizeof(int));
    start = rdtsc_start();
    read(myPipe[0], &temp, sizeof(temp));
    end = rdtsc_end();
    return end - start;
}

double getThreadSwitchContextOverhead() {
    double sum = 0.0;
    int times = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        double readwrite = getThreadRead();
        double readwriteswitch = getThreadSwitch();
//        printf("read write: %f\n", readwrite);
//        printf("read write switch: %f\n", readwriteswitch);
        if (readwriteswitch - readwrite > 0) {
            sum += (readwriteswitch - readwrite);
            ++times;
        }
    }
    return sum / times;
}
void calculateMeanVar(std::vector<double> res) {
    double sum = 0, varSum = 0;
    double mean, var;
    for (int i=0;i<REPEAT_TIMES; ++i) {
        sum += res[i];
    }
    mean = sum / (double)REPEAT_TIMES;
    for (int i=0;i<REPEAT_TIMES; ++i) {
        varSum += (res[i] - mean) * (res[i] - mean);
    }
    var = varSum / (double)REPEAT_TIMES;
    printf("Mean is %f, variance is %f, standard deviation is %f\n", mean, var, sqrt(var));
}

int main() {
    std::vector<double> readOverhead;
    for (int i=0; i<REPEAT_TIMES; ++i) {
        readOverhead.push_back(getReadOverhead());
        printf("read overhead for %d times is %f\n", i, readOverhead[i]);
    }
    calculateMeanVar(readOverhead);

    std::vector<double> loopOverhead;
    for (int i=0; i<REPEAT_TIMES; ++i) {
        loopOverhead.push_back(getLoopOverhead());
        printf("loop overhead for %d times is %f\n", i, loopOverhead[i]);
    }
    calculateMeanVar(loopOverhead);

    std::vector<double> procedureOverhead;
    getProcedureOverhead(procedureOverhead);
    for (int i=0; i<8; ++i) {
        printf("The average overhead of a function with %d integer arguments is %f\n", i, procedureOverhead[i]);
    }

    std::vector<double> systemCallOverhead;
    getSystemCallOverhead();
    for (int i=0; i<REPEAT_TIMES; ++i) {
        systemCallOverhead.push_back(getSystemCallOverhead());
        printf("The average overhead of a system call with %d integer arguments is %f\n", i, systemCallOverhead[i]);
    }
    calculateMeanVar(systemCallOverhead);

    std::vector<double> threadCreationOverhead;
    for (int i=0; i<REPEAT_TIMES; ++i) {
        threadCreationOverhead.push_back(getThreadCreationOverhead());
        printf("thread creation overhead for %d times is %f\n", i, threadCreationOverhead[i]);
    }
    calculateMeanVar(threadCreationOverhead);

    std::vector<double> processCreationOverhead;
    for (int i=0; i<REPEAT_TIMES; ++i) {
        processCreationOverhead.push_back(getProcessCreationOverhead());
        printf("process creation overhead for %d times is %f\n", i, processCreationOverhead[i]);
    }
    calculateMeanVar(processCreationOverhead);

    std::vector<double> processSwitchOverhead;
    for (int i=0; i<REPEAT_TIMES; ++i) {
        processSwitchOverhead.push_back(getProcessSwitchContextOverhead());
        printf("process creation overhead for %d times is %f\n", i, processSwitchOverhead[i]);
    }
    calculateMeanVar(processSwitchOverhead);

    std::vector<double> threadSwitchOverhead;
    for (int i=0; i<REPEAT_TIMES; ++i) {
        threadSwitchOverhead.push_back(getThreadSwitchContextOverhead());
        printf("thread creation overhead for %d times is %f\n", i, threadSwitchOverhead[i]);
    }

    calculateMeanVar(threadSwitchOverhead);
    //printf("context switch between threads is %f\n", getThreadSwitchContextOverhead());
    return 0;
}
