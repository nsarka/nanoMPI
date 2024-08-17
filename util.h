#include "mpi.h"

static inline int nanompi_get_dtype_size(MPI_Datatype dtype)
{
    return dtype.size;
}
