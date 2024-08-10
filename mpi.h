#ifndef NANOMPI_MPI_H
#define NANOMPI_MPI_H

#include <stddef.h>
#include <stdint.h>

#include "comm.h"
#include "proc.h"
#include "group.h"
#include "status.h"

#define MPI_VERSION 3
#define MPI_SUBVERSION 1

#define MPI_Datatype size_t
//typedef struct nanompi_op_t *MPI_Op;

int MPI_Init(int *argc, char ***argv);
int MPI_Finalize(void);

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                            int tag, MPI_Comm comm);
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
                            int tag, MPI_Comm comm, MPI_Status *status);

/*
 * Error classes and codes
 */
#define MPI_SUCCESS                   0
#define MPI_ERR_OTHER                 16
#define MPI_ERR_UNSUPPORTED_OPERATION 52

/* Predefined datatypes */
#define MPI_CHAR sizeof(char)
#define MPI_INT sizeof(int)
#define MPI_FLOAT sizeof(float)

#endif
