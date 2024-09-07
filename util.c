#include "util.h"
#include <time.h>

struct timespec start;

void nanompi_init_clock()
{
    clock_gettime(CLOCK_MONOTONIC, &start);
}

double MPI_Wtime(void)
{
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    return end.tv_sec - start.tv_sec;
}
