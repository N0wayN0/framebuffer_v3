#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

//static long long start_time;
//static long long end_time;

long long start_here(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    start_time = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void end_here(const char *label) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    end_time = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
    printf("%s %lld ns (%.3f ms) \n", label, end_time - start_time, (end_time - start_time) / 1e6);
}


