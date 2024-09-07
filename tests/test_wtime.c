#include <stdio.h>
#include <unistd.h>
#include "mpi.h"

int main(int argc, char** argv) {
    double start;
    MPI_Init(&argc, &argv);
    start = MPI_Wtime();
    sleep(2);
    printf("2 second sleep measured to be %f seconds\n", MPI_Wtime() - start);
    MPI_Finalize();
    return 0;
}