#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

MPI_Comm nanompi_comm_world;

int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
    *rank = comm->my_rank;
    return MPI_SUCCESS;
}

int MPI_Comm_size(MPI_Comm comm, int *size)
{
    *size = comm->local_group->grp_proc_count;
    return MPI_SUCCESS;
}

int nanompi_init_comm(nanompi_communicator_t **comm_dptr, int rank, int world_size, char *hostfile)
{
    int status = MPI_SUCCESS;

    nanompi_comm_world = malloc(sizeof(nanompi_communicator_t));
    if (!nanompi_comm_world) {
        printf("Error allocating nanompi_communicator_t\n");
        return MPI_ERR_OTHER;
    }

    status = nanompi_init_group(&nanompi_comm_world->local_group, rank, world_size, hostfile);
    if (status) {
        printf("error in nanompi_init_group: %d\n", status);
        goto free_comm_world;
    }

    nanompi_comm_world->my_rank = rank;

exit:
    return status;
free_comm_world:
    free(nanompi_comm_world);
    goto exit;
}

int nanompi_free_comm(nanompi_communicator_t *comm)
{
    int status;

    status = nanompi_free_group(comm->local_group);
    if (status) {
        printf("Error freeing comm->local_group\n");
        return status;
    }

    free(comm);
}
