#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"


int MPI_Init(int *argc, char ***argv)
{
    int my_rank = atoi(getenv("NANOMPI_WORLD_RANK"));
    int world_size = atoi(getenv("NANOMPI_WORLD_SIZE"));
    char *hostfile = getenv("NANOMPI_HOSTFILE");

    nanompi_comm_world = malloc(sizeof(nanompi_communicator_t));
    nanompi_comm_world->local_group = malloc(sizeof(nanompi_group_t));

    nanompi_comm_world->local_group->grp_my_rank = my_rank;
    nanompi_comm_world->local_group->grp_proc_count = world_size;

    nanompi_comm_world->my_rank = my_rank;

    return MPI_SUCCESS;
}

int MPI_Finalize(void)
{
    free(nanompi_comm_world->local_group);
    free(nanompi_comm_world);
    return MPI_SUCCESS;
}

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                            int tag, MPI_Comm comm)
{
    return MPI_SUCCESS;
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
                            int tag, MPI_Comm comm, MPI_Status *status)
{
    return MPI_SUCCESS;
}
/*
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
        memcpy(recvbuf, sendbuf, count * datatype);
        for (int i = 0; i < size; i++) {
            if (i != root) {
                void *tempbuf = malloc(count * datatype);
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

    void *tempbuf = malloc(total_count * datatype);
    MPI_Allreduce(sendbuf, tempbuf, total_count, datatype, op, comm);

    int offset = 0;
    for (int i = 0; i < rank; i++) {
        offset += recvcounts[i];
    }

    memcpy(recvbuf, (char*)tempbuf  offset * datatype, recvcounts[rank] * datatype);
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
                MPI_Send((char*)sendbuf  i * sendcount * sendtype, sendcount, sendtype, i, 0, comm);
            } else {
                memcpy(recvbuf, (char*)sendbuf  i * sendcount * sendtype, recvcount * recvtype);
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
                MPI_Recv((char*)recvbuf  i * recvcount * recvtype, recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
            } else {
                memcpy((char*)recvbuf  i * recvcount * recvtype, sendbuf, sendcount * sendtype);
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

int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    for (int i = 0; i < size; i++) {
        MPI_Send((char*)sendbuf  i * sendcount * sendtype, sendcount, sendtype, i, 0, comm);
        MPI_Recv((char*)recvbuf  i * recvcount * recvtype, recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
    }

    return MPI_SUCCESS;
}
*/
