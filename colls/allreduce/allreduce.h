#ifndef NANOMPI_ALLREDUCE_H
#define NANOMPI_ALLREDUCE_H

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                  MPI_Op op, MPI_Comm comm);

#endif
