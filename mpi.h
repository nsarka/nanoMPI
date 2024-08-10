#include <stddef.h>
#include <stdint.h>

#include "comm.h"
#include "proc.h"
#include "group.h"
#include "status.h"

#define MPI_VERSION 3
#define MPI_SUBVERSION 1

typedef struct nanompi_communicator_t *MPI_Comm;
#define MPI_Datatype size_t
typedef struct nanompi_status_public_t MPI_Status;
//typedef struct nanompi_op_t *MPI_Op;

int MPI_Init(int *argc, char ***argv);
int MPI_Finalize(void);

int MPI_Comm_rank(MPI_Comm comm, int *rank);
int MPI_Comm_size(MPI_Comm comm, int *size);

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

/* Comm world */
extern MPI_Comm nanompi_comm_world;
#define MPI_COMM_WORLD nanompi_comm_world

/* Predefined datatypes */
#define MPI_CHAR sizeof(char)
#define MPI_INT sizeof(int)
#define MPI_FLOAT sizeof(float)