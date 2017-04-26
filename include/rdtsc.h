//
// Created by Xingdong Li on 4/23/17.
//

#ifndef HAVE_RDTSC_H
#define HAVE_RDTSC_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


static __inline__ unsigned long long rdtsc_start(void)
{
    unsigned cycles_high0, cycles_low0;
    asm volatile (
            "CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
    : "=r" (cycles_high0), "=r" (cycles_low0)
    :: "%rax", "%rbx", "%rcx", "%rdx");
    return ((unsigned long long)cycles_low0)|( ((unsigned long long)cycles_high0)<<32 );
}

static __inline__ unsigned long long rdtsc_end(void)
{
    unsigned cycles_high1, cycles_low1;
    asm volatile (
            "RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t"
    : "=r" (cycles_high1), "=r" (cycles_low1)
    :: "%rax", "%rbx", "%rcx", "%rdx");
    return ((unsigned long long)cycles_low1)|( ((unsigned long long)cycles_high1)<<32 );
}


#endif //HAVE_RDTSC_H
