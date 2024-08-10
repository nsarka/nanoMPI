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

#endif
