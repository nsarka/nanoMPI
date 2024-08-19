// colls/allreduce.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "util.h"

// Helper function for element-wise sum (you can extend this for other operations)
static void sum_elements(void *in, void *inout, int *len, MPI_Datatype *dtype)
{
    int count = *len;
    if (*dtype == MPI_INT) {
        int *a = (int *)in;
        int *b = (int *)inout;
        for (int i = 0; i < count; i++)
            b[i] += a[i];
    }
    // TODO: Add more data types?
}

// TODO: Add more reduction ops?

int MPI_Allreduce_ring(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                       MPI_Op op, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int type_size = nanompi_get_dtype_size(datatype);

    // Copy sendbuf to recvbuf
    memcpy(recvbuf, sendbuf, count * type_size);

    void *temp_buf = malloc(count * type_size);
    void *send_buf = malloc(count * type_size);
    memcpy(send_buf, recvbuf, count * type_size);

    for (int i = 0; i < size - 1; i++) {
        int send_to = (rank + 1) % size;
        int recv_from = (rank - 1 + size) % size;

        MPI_Send(send_buf, count, datatype, send_to, 0, comm);
        MPI_Recv(temp_buf, count, datatype, recv_from, 0, comm, MPI_STATUS_IGNORE);

        // Perform reduction operation
        sum_elements(temp_buf, recvbuf, count, datatype);

        // Prepare for next iteration
        memcpy(send_buf, temp_buf, count * type_size);
    }

    free(temp_buf);
    free(send_buf);
    return MPI_SUCCESS;
}

int MPI_Allreduce_tree(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                       MPI_Op op, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int type_size;
    MPI_Type_size(datatype, &type_size);

    // Copy sendbuf to recvbuf
    memcpy(recvbuf, sendbuf, count * type_size);

    void *temp_buf = malloc(count * type_size);

    // Reduction phase (up the tree)
    for (int stride = 1; stride < size; stride *= 2) {
        if ((rank % (2 * stride)) == 0) {
            if (rank + stride < size) {
                MPI_Recv(temp_buf, count, datatype, rank + stride, 0, comm, MPI_STATUS_IGNORE);
                sum_elements(temp_buf, recvbuf, &count, &datatype);
            }
        } else {
            MPI_Send(recvbuf, count, datatype, rank - stride, 0, comm);
            break;
        }
    }

    // Broadcast phase (down the tree)
    for (int stride = size / 2; stride > 0; stride /= 2) {
        if ((rank % (2 * stride)) == 0) {
            if (rank + stride < size) {
                MPI_Send(recvbuf, count, datatype, rank + stride, 0, comm);
            }
        } else {
            MPI_Recv(recvbuf, count, datatype, rank - stride, 0, comm, MPI_STATUS_IGNORE);
            break;
        }
    }

    free(temp_buf);
    return MPI_SUCCESS;
}

int MPI_Allreduce_reduce_bcast(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                               MPI_Op op, MPI_Comm comm)
{
    MPI_Reduce(sendbuf, recvbuf, count, datatype, op, 0, comm);
    MPI_Bcast(recvbuf, count, datatype, 0, comm);

    return MPI_SUCCESS;
}

int MPI_Allreduce_reduce_scatter_allgather(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                                           MPI_Op op, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int type_size;
    MPI_Type_size(datatype, &type_size);

    int *recvcounts = (int *)malloc(size * sizeof(int));
    int *displs = (int *)malloc(size * sizeof(int));
    int chunk_size = count / size;
    int remainder = count % size;

    for (int i = 0; i < size; i++) {
        recvcounts[i] = chunk_size + (i < remainder ? 1 : 0);
        displs[i] = (i > 0) ? (displs[i-1] + recvcounts[i-1]) : 0;
    }

    void *temp_buf = malloc(count * type_size);

    // Reduce-scatter
    MPI_Reduce_scatter(sendbuf, temp_buf, recvcounts, datatype, op, comm);

    // Allgather
    MPI_Allgatherv(temp_buf, recvcounts[rank], datatype,
                   recvbuf, recvcounts, displs, datatype, comm);

    free(recvcounts);
    free(displs);
    free(temp_buf);

    return MPI_SUCCESS;
}
