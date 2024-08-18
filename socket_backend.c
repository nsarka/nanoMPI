#include "socket_backend.h"
#include "constants.h"

#include "comm.h"

int nanompi_init_socket_backend(nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;
    int rank = comm->my_rank;
    int size = comm->local_group->grp_proc_count;
    nanompi_proc_t **grp_proc_pointers = comm->local_group->grp_proc_pointers;

    

    return status;
}

int nanompi_init_free_backend(nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;

    return status;
}
