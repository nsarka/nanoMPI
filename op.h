#ifndef NANOMPI_OP_H
#define NANOMPI_OP_H

#include "mpi.h"

enum nanompi_op_type {
    NANOMPI_OP_NULL,
    NANOMPI_OP_MAX,
    NANOMPI_OP_MIN,
    NANOMPI_OP_SUM,
    NANOMPI_OP_PROD,
    NANOMPI_OP_LAND,
    NANOMPI_OP_BAND,
    NANOMPI_OP_LOR,
    NANOMPI_OP_BOR,
    NANOMPI_OP_LXOR,
    NANOMPI_OP_BXOR,
    NANOMPI_OP_MAXLOC,
    NANOMPI_OP_MINLOC,
    NANOMPI_OP_REPLACE,
    NANOMPI_OP_NUM_OF_TYPES
};

struct nanompi_op_t {
    enum ompi_op_type op_type;
    nanompi_op_fn_t *c_fn;
};

typedef struct nanompi_op_t *MPI_Op;

extern MPI_Op nanompi_op_sum;

#define MPI_SUM nanompi_op_sum