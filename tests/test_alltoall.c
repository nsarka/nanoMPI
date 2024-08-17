#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define ARRAY_SIZE 16

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data[ARRAY_SIZE];
    int recv_data[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++) {
        send_data[i] = rank * 100 + i;
    }

    printf("Rank %d: Send data: ", rank);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", send_data[i]);
    }
    printf("\n");

    MPI_Alltoall(send_data, ARRAY_SIZE / size, MPI_INT, recv_data, ARRAY_SIZE / size, MPI_INT, MPI_COMM_WORLD);

    printf("Rank %d: Received data: ", rank);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}