#include "op.h"

#include <stdio.h>

static void op_sum(const void *in, void *out, int *count, MPI_Datatype *dtype)
{
    switch (dtype->id) {
        case NANOMPI_DTYPE_INT:
        {
            const int *ptr_in = in;
            int *ptr_out = out;
            for (int i = *count; i >= 0; i--) {
                ptr_out[i] = ptr_out[i] + ptr_in[i];
            }
        }
        case NANOMPI_DTYPE_FLOAT:
        {
            const float *ptr_in = in;
            float *ptr_out = out;
            for (int i = *count; i >= 0; i--) {
                ptr_out[i] = ptr_out[i] + ptr_in[i];
            }
        }
        case NANOMPI_DTYPE_CHAR:
        {
            const char *ptr_in = in;
            char *ptr_out = out;
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

nanompi_op_t nanompi_op_sum =
{
    .op_type = NANOMPI_OP_SUM,
    .fn_ptr = op_sum
};
