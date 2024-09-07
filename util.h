#ifndef NANOMPI_UTIL_H
#define NANOMPI_UTIL_H

#include "mpi.h"

static inline int nanompi_get_dtype_size(MPI_Datatype dtype)
{
    return dtype.size;
}

void nanompi_init_clock();
double MPI_Wtime(void);

#endif
