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
