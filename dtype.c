#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

nanompi_dtype_t nanompi_dtype_int =
{
    .id = NANOMPI_DTYPE_INT,
    .size = sizeof(int)
};

nanompi_dtype_t nanompi_dtype_float =
{
    .id = NANOMPI_DTYPE_FLOAT,
    .size = sizeof(float)
};

nanompi_dtype_t nanompi_dtype_char =
{
    .id = NANOMPI_DTYPE_CHAR,
    .size = sizeof(char)
};
