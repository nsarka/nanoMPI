#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"
#include "util.h"
#include "socket_backend.h"

int MPI_Init(int *argc, char ***argv)
{
    int status = MPI_SUCCESS;
    int my_rank = atoi(getenv("NANOMPI_WORLD_RANK"));
    int world_size = atoi(getenv("NANOMPI_WORLD_SIZE"));
    char *hostfile = getenv("NANOMPI_HOSTFILE");

    status = nanompi_init_comm(&nanompi_comm_world, my_rank, world_size, hostfile);
    if (status) {
        printf("Error in nanompi_init_comm\n");
    }

    return status;
}

int MPI_Finalize(void)
{
    int status;
    
    status = nanompi_free_comm(nanompi_comm_world);
    if (status) {
        printf("Error in nanompi_free_comm\n");
    }
    
    return status;
}

// TODO: tags
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
    int status = MPI_SUCCESS;
    size_t msg_size = count * nanompi_get_dtype_size(datatype);
    status = nanompi_socket_send(buf, msg_size, dest, comm); // for now, ignore tag
    return status;
}

// TODO: tags
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *st)
{
    int status = MPI_SUCCESS;
    size_t msg_size = count * nanompi_get_dtype_size(datatype);
    status = nanompi_socket_recv(buf, msg_size, source, comm);
    if(st) {
        st->MPI_ERROR = status;
        st->MPI_SOURCE = source;
        st->MPI_TAG = tag;
    }
    return status;
}

int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(buffer, count, datatype, i, 0, comm);
            }
        }
    } else {
        MPI_Recv(buffer, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
    }

    return MPI_SUCCESS;
}

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        memcpy(recvbuf, sendbuf, count * nanompi_get_dtype_size(datatype));
        for (int i = 0; i < size; i++) {
            if (i != root) {
                void *tempbuf = malloc(count * nanompi_get_dtype_size(datatype));
                MPI_Recv(tempbuf, count, datatype, i, 0, comm, MPI_STATUS_IGNORE);
                // TODO: other reduction ops
                for (int j = 0; j < count; j++) {
                    ((int*)recvbuf)[j] += ((int*)tempbuf)[j];
                }
                free(tempbuf);
            }
        }
    } else {
        MPI_Send(sendbuf, count, datatype, root, 0, comm);
    }

    return MPI_SUCCESS;
}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                  MPI_Op op, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    MPI_Reduce(sendbuf, recvbuf, count, datatype, op, 0, comm);
    MPI_Bcast(recvbuf, count, datatype, 0, comm);

    return MPI_SUCCESS;
}

int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int total_count = 0;
    for (int i = 0; i < size; i++) {
        total_count += recvcounts[i];
    }

    void *tempbuf = malloc(total_count * nanompi_get_dtype_size(datatype));
    MPI_Allreduce(sendbuf, tempbuf, total_count, datatype, op, comm);

    int offset = 0;
    for (int i = 0; i < rank; i++) {
        offset += recvcounts[i];
    }

    memcpy(recvbuf, (char*)tempbuf + offset * nanompi_get_dtype_size(datatype), recvcounts[rank] * nanompi_get_dtype_size(datatype));
    free(tempbuf);

    return MPI_SUCCESS;
}

int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send((char*)sendbuf + i * sendcount * nanompi_get_dtype_size(sendtype), sendcount, sendtype, i, 0, comm);
            } else {
                memcpy(recvbuf, (char*)sendbuf + i * sendcount * nanompi_get_dtype_size(sendtype), recvcount * nanompi_get_dtype_size(recvtype));
            }
        }
    } else {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, MPI_STATUS_IGNORE);
    }

    return MPI_SUCCESS;
}

int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Recv((char*)recvbuf + i * recvcount * nanompi_get_dtype_size(recvtype), recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
            } else {
                memcpy((char*)recvbuf + i * recvcount * nanompi_get_dtype_size(recvtype), sendbuf, sendcount * nanompi_get_dtype_size(sendtype));
            }
        }
    } else {
        MPI_Send(sendbuf, sendcount, sendtype, root, 0, comm);
    }

    return MPI_SUCCESS;
}

int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    for (int root = 0; root < size; root++) {
        MPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
    }

    return MPI_SUCCESS;
}

int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const int *recvcounts, const int *displs,
                   MPI_Datatype recvtype, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // First, gather all data to root (rank 0)
    MPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcounts, recvtype, 0, comm);

    // Then, broadcast the gathered data to all processes
    int total_recv_count = 0;
    for (int i = 0; i < size; i++) {
        total_recv_count += recvcounts[i];
    }

    MPI_Bcast(recvbuf, total_recv_count, recvtype, 0, comm);

    // If the receiving buffer is not properly displaced, we need to rearrange it
    if (rank != 0) {
        void *temp_buf = malloc(total_recv_count * nanompi_get_dtype_size(recvtype));
        memcpy(temp_buf, recvbuf, total_recv_count * nanompi_get_dtype_size(recvtype));

        for (int i = 0; i < size; i++) {
            memcpy((char*)recvbuf + displs[i] * nanompi_get_dtype_size(recvtype),
                   (char*)temp_buf + (displs[i] * nanompi_get_dtype_size(recvtype)),
                   recvcounts[i] * nanompi_get_dtype_size(recvtype));
        }

        free(temp_buf);
    }

    return MPI_SUCCESS;
}

int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    for (int i = 0; i < size; i++) {
        MPI_Send((char*)sendbuf + i * sendcount * nanompi_get_dtype_size(sendtype), sendcount, sendtype, i, 0, comm);
        MPI_Recv((char*)recvbuf + i * recvcount * nanompi_get_dtype_size(recvtype), recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
    }

    return MPI_SUCCESS;
}
