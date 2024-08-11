#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define ARRAY_SIZE 5

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[ARRAY_SIZE];
    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            data[i] = i * 10;
        }
        printf("Rank 0: Broadcasting data\n");
    }

    MPI_Bcast(data, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Rank %d received: ", rank);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}