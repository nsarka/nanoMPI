#ifndef NANOMPI_UTIL_H
#define NANOMPI_UTIL_H

#include "mpi.h"

static inline int nanompi_get_dtype_size(MPI_Datatype dtype)
{
    return dtype.size;
}

static inline size_t nanompi_get_msg_size(MPI_Datatype dtype, int count)
{
    return count * nanompi_get_dtype_size(dtype);
}

void nanompi_init_clock();

double MPI_Wtime(void);
int MPI_Abort(MPI_Comm comm, int errorcode);

#endif
