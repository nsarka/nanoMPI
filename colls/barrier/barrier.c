#include "mpi.h"
#include "util.h"

int MPI_Barrier_basic(MPI_Comm comm)
{
    int rank, size, send_to, recv_from;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    send_to = (rank + 1) % size;
    recv_from = (rank - 1 + size) % size;

    MPI_Send(NULL, 0, MPI_BYTE, send_to, 0, comm);
    MPI_Recv(NULL, 0, MPI_BYTE, recv_from, 0, comm, MPI_STATUS_IGNORE);

    return MPI_SUCCESS;
}
