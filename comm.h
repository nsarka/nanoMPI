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

    /* Collectives module interface and data */
    //mca_coll_base_comm_coll_t *c_coll;

    /* instance that this comm belongs to */
    //nanompi_instance_t* instance;
};
typedef struct nanompi_communicator_t nanompi_communicator_t;

#endif
