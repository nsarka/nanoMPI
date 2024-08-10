#ifndef NANOMPI_COMM_H
#define NANOMPI_COMM_H

#include "mpi.h"
#include "group.h"

struct nanompi_communicator_t {
    int                           my_rank;
    uint32_t                      flags; /* flags, e.g. intercomm,
                                              topology, etc. */
    uint32_t                      assertions; /* info assertions */

    nanompi_group_t        *local_group;
};
typedef struct nanompi_communicator_t nanompi_communicator_t;

typedef struct nanompi_communicator_t *MPI_Comm;

/* Comm world */
extern MPI_Comm nanompi_comm_world;
#define MPI_COMM_WORLD nanompi_comm_world

int MPI_Comm_rank(MPI_Comm comm, int *rank);
int MPI_Comm_size(MPI_Comm comm, int *size);

#endif
