#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include "time_ms.h"

int throwLoop(int throwCount) {
    double x, y;

    int totalHits = 0;
#pragma omp parallel reduction(+:totalHits)
    {
        int hitCount = 0;
        unsigned int myseed = omp_get_thread_num();
#pragma omp for schedule(static)
        for (int i = 0; i < throwCount; i++) {
            x = (double) (double) rand_r(&myseed) / RAND_MAX;
            y = (double) (double) rand_r(&myseed) / RAND_MAX;

            if ((x * x + y * y <= 1))
                hitCount++;
        }
        printf("found %d hits\n", hitCount);
        totalHits+=hitCount;
    }
    return totalHits;
}

double getResult(int throwCount, int hitCount) {
    return (double) hitCount / throwCount * 4;
}

int main(int argc, char **argv) {
    int throwCount = 10000000;
    unsigned long start_time = time_ms();
    int hitCount = throwLoop(throwCount);
    printf("Pi seems to be %f\n", getResult(throwCount, hitCount));
    unsigned long end_time = time_ms() - start_time;
    printf("Took %lu ms using %s threads\n", end_time, argv[1]);
}