#include "util.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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

int MPI_Abort(MPI_Comm comm, int errorcode)
{
    int rank;
    MPI_Comm_rank(comm, &rank);
    printf("rank %d called MPI_Abort, exiting...\n", rank);
    exit(errorcode);
}
