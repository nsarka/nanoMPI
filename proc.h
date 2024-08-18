#ifndef NANOMPI_PROC_H
#define NANOMPI_PROC_H

#include "constants.h"

typedef struct nanompi_proc_t {
    char hostname[MAX_HOSTNAME_LENGTH];
    int port;

    /* endpoint data */
    //void *proc_endpoints[OMPI_PROC_ENDPOINT_TAG_MAX];
} nanompi_proc_t;

#endif