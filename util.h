#include "mpi.h"

int nanompi_get_dtype_size(MPI_Datatype *dtype)
{
    return dtype->size;
}
