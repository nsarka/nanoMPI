#include "util.h"
#include <time.h>

struct timespec start, end;

void nanompi_init_clock()
{
    clock_gettime(CLOCK_MONOTONIC, &start);
}

double MPI_Wtime(void)
{
    clock_gettime(CLOCK_MONOTONIC, &end);
    return end.tv_sec - start.tv_sec;
}
