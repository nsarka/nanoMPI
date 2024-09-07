#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_RUNS 100
#define MAX_MESSAGE_SIZE 1048576  // 1 MB
#define MIN_MESSAGE_SIZE 8        // 8 bytes

double benchmark_allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    // Warm-up run
    MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);

    // Synchronize all processes
    MPI_Barrier(comm);

    double start_time = MPI_Wtime();

    for (int i = 0; i < NUM_RUNS; i++) {
        MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
    }

    double end_time = MPI_Wtime();

    // Average time per operation in microseconds
    return (end_time - start_time) * 1e6 / NUM_RUNS;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allocate buffers for the maximum message size
    double* sendbuf = (double*)malloc(MAX_MESSAGE_SIZE);
    double* recvbuf = (double*)malloc(MAX_MESSAGE_SIZE);

    if (sendbuf == NULL || recvbuf == NULL) {
        fprintf(stderr, "Error: Could not allocate memory\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize send buffer
    for (int i = 0; i < MAX_MESSAGE_SIZE / sizeof(double); i++) {
        sendbuf[i] = rank + 1;
    }

    if (rank == 0) {
        printf("Message Size (bytes)\tTime (microseconds)\n");
    }

    for (int message_size = MIN_MESSAGE_SIZE; message_size <= MAX_MESSAGE_SIZE; message_size *= 2) {
        int count = message_size / sizeof(double);
        double time = benchmark_allreduce(sendbuf, recvbuf, count, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // Only the root process prints the results
        if (rank == 0) {
            printf("%d\t\t\t%.2f\n", message_size, time);
        }
    }

    free(sendbuf);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}
