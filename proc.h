#ifndef NANOMPI_PROC_H
#define NANOMPI_PROC_H

#include "mpi.h"

struct nanompi_proc_t {
    char hostname[128];
    int port;

    /* endpoint data */
    //void *proc_endpoints[OMPI_PROC_ENDPOINT_TAG_MAX];
};

typedef struct nanompi_proc_t nanompi_proc_t;

#endif