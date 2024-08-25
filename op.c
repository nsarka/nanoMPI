#include "op.h"

#include <stdio.h>

static void op_sum(const void *in, void *out, int *count, MPI_Datatype *dtype)
{
    switch (dtype->id) {
        case NANOMPI_DTYPE_INT:
        {
            const int *ptr_in = (int*) in;
            int *ptr_out = (int*) out;
            for (int i = *count; i >= 0; i--) {
                ptr_out[i] = ptr_out[i] + ptr_in[i];
            }
        }
        case NANOMPI_DTYPE_FLOAT:
        {
            const float *ptr_in = (float*) in;
            float *ptr_out = (float*) out;
            for (int i = *count; i >= 0; i--) {
                ptr_out[i] = ptr_out[i] + ptr_in[i];
            }
        }
        case NANOMPI_DTYPE_CHAR:
        {
            const char *ptr_in = (char*) in;
            char *ptr_out = (char*) out;
            for (int i = *count; i >= 0; i--) {
                ptr_out[i] = ptr_out[i] + ptr_in[i];
            }
        }
        default:
        {
            printf("dtype not supported yet\n");
        }
    }

}

MPI_Op nanompi_op_sum =
{
    .op_type = NANOMPI_OP_SUM,
    .fn_ptr = op_sum
};
