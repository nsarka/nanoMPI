#ifndef NANOMPI_GROUP_H
#define NANOMPI_GROUP_H

#include "mpi.h"

struct nanompi_group_t {
    int grp_proc_count;     /**< number of processes in group */
    int grp_my_rank;        /**< rank in group */
    struct nanompi_proc_t **grp_proc_pointers;
                            /**< list of pointers to ompi_proc_t structures
                                 for each process in the group */
    //uint32_t grp_flags;     /**< flags, e.g. freed, cannot be freed etc.*/
    /** pointer to the original group when using sparse storage */
    struct nanompi_group_t *grp_parent_group_ptr;
};

typedef struct nanompi_group_t nanompi_group_t;

#endif
