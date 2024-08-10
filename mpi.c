#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

MPI_Comm nanompi_comm_world;

int MPI_Init(int *argc, char ***argv)
{
    int my_rank = atoi(getenv("NANOMPI_WORLD_RANK"));
    int world_size = atoi(getenv("NANOMPI_WORLD_SIZE"));
    nanompi_comm_world = malloc(sizeof(nanompi_communicator_t));
    nanompi_comm_world->local_group = malloc(sizeof(nanompi_group_t));
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
