//
// Created by Xingdong Li on 4/22/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "rdtsc.h"
#include "utilities.h"
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

#define LOOP_TIMES 100
#define REPEAT_TIMES 100
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

    start = rdtsc_start();
    getpid();
    end = rdtsc_end();
    return (double)(end - start);
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

double getReadWriteSwitch(int* myPipe) {
    uint64_t start, end;
    int temp = 1;
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        return -1;
    }
    if (pid != 0) {
        start = rdtsc_start();
        wait(NULL);
        read(myPipe[0], &temp, sizeof(int));
        end = rdtsc_end();
    }
    else {
        write(myPipe[1], &temp, sizeof(int));
        exit(1);
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
    wait(NULL);
    read(myPipe[0], &temp, sizeof(int));
    end = rdtsc_end();
    return (double)(end - start);
}

double getProcessSwitchContextOverhead() {
    int myPipe[2];
    pipe(myPipe);
    double sum = 0;

    for (int i=0; i < 100; ++i) {
        double readwrite = getReadWrite(myPipe);
        double readwriteswitch = getReadWriteSwitch(myPipe);
        printf("read write: %f\n", readwrite);
        printf("read write switch: %f\n", readwriteswitch);
        sum = (readwriteswitch - readwrite);
        printf("%f\n", sum);
    }

    return sum / 1;
}

double getRead(int *myPipe) {
    uint64_t start, end;
    double sum = 0;
    int temp = 1;
    for (int i=0; i<LOOP_TIMES; ++i) {
        start = rdtsc_start();
        read(myPipe[0], &temp, sizeof(int));
        end = rdtsc_end();
        sum += (end - start);
    }
    return sum / (double) LOOP_TIMES;
}

double getThreadSwitchContextOverhead() {
    int myPipe[2];
    pipe(myPipe);
    double sum = 0;

    uint64_t start, end;
    pthread_t threadId;
    double sum = 0;
    for (int i=0; i<LOOP_TIMES; ++i) {
        pthread_create(&threadId, NULL, newThread, NULL);
        start = rdtsc_start();
        read(myPipe[0], &end, sizeof(end));
        pthread_join(threadId, NULL);
        sum += (end - start);
    }
    double readTime = getRead(myPipe);
    return sum / (double)LOOP_TIMES - readTime;
}

void calculateMeanVar(int *res) {
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
    printf("Mean is %f, variance is %f\n", mean, var);
}

int main() {
    int *readOverhead = new int(REPEAT_TIMES);
    for (int i=0; i<REPEAT_TIMES; ++i) {
        readOverhead[i] = getReadOverhead();
        printf("read overhead for %d times is %f\n", i, readOverhead[i]);
    }
    calculateMeanVar(readOverhead);

    int *loopOverhead = new int(REPEAT_TIMES);
    for (int i=0; i<REPEAT_TIMES; ++i) {
        loopOverhead[i] = getLoopOverhead();
        printf("loop overhead for %d times is %f\n", i, loopOverhead[i]);
    }
    calculateMeanVar(loopOverhead);

    std::vector<double> procedureOverhead;
    getProcedureOverhead(procedureOverhead);
    for (int i=0; i<8; ++i) {
        printf("The average overhead of a function with %d integer arguments is %f\n", i, procedureOverhead[i]);
    }

    printf("system call overhead is %f\n", getSystemCallOverhead());

    int *threadCreationOverhead = new int(REPEAT_TIMES);
    for (int i=0; i<REPEAT_TIMES; ++i) {
        threadCreationOverhead[i] = getThreadCreationOverhead();
        printf("thread creation overhead for %d times is %f\n", i, threadCreationOverhead[i]);
    }
    calculateMeanVar(threadCreationOverhead);

    int *processCreationOverhead = new int(REPEAT_TIMES);
    for (int i=0; i<REPEAT_TIMES; ++i) {
        processCreationOverhead[i] = getProcessCreationOverhead();
        printf("process creation overhead for %d times is %f\n", i, processCreationOverhead[i]);
    }
    calculateMeanVar(processCreationOverhead);

    printf("context switch between processes is %f\n", getSwitchContextOverhead());
    printf("context switch between threads is %f\n", getThreadSwitchContextOverhead());

    int *threadSwitchOverhead = new int(REPEAT_TIMES);
    for (int i=0; i<REPEAT_TIMES; ++i) {
        threadSwitchOverhead[i] = getThreadSwitchContextOverhead();
        printf("process creation overhead for %d times is %f\n", i, threadSwitchOverhead[i]);
    }
    calculateMeanVar(threadSwitchOverhead);

    return 0;
}
