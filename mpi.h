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
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm);
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                  MPI_Op op, MPI_Comm comm);
int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);

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

/* Predefined reduction operations */
typedef int MPI_Op;
#define MPI_SUM 0
#define MPI_MAX 1
#define MPI_MIN 2
#define MPI_PROD 3

#endif
