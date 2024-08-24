#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#include "socket_backend.h"
#include "constants.h"

#include "comm.h"


static int init_server(nanompi_communicator_t *comm)
{
    struct sockaddr_in address;
    int status = MPI_SUCCESS;
    int rank = comm->my_rank;
    int size = comm->local_group->grp_proc_count;
    nanompi_proc_t **grp_proc_pointers = comm->local_group->grp_proc_pointers;
    int addrlen = sizeof(address);
    int opt = 1;
    int i;

    // Create socket file descriptor
    if ((comm->socket_info.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        status = MPI_ERR_OTHER;
        goto exit;
    }

    // Reuse addr and port to avoid pesky bind: address in use errors >:(
    // This is especially useful right now because the job launcher does not forward Ctrl+C signals to each process.
    // That means after every run that errors or is Ctrl+C'd out, we get more stray processes
    // TODO: fix this
    if (setsockopt(comm->socket_info.server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        status = MPI_ERR_OTHER;
        goto close;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(grp_proc_pointers[rank]->port);

    if (bind(comm->socket_info.server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("server: bind failed");
        status = MPI_ERR_OTHER;
        goto close;
    }

    if (listen(comm->socket_info.server_fd, size) < 0) {
        perror("server: listen");
        status = MPI_ERR_OTHER;
        goto close;
    }

    for(i = rank; i >= 0; i--) {
        // We dont actually know what rank is connecting to us. Just pretend it's in rank-order for now and sort client_fds later
        comm->socket_info.client_fds[i] = accept(comm->socket_info.server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    }

exit:
    return status;
close:
    close(comm->socket_info.server_fd);
    goto exit;
}

static int init_clients(nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;
    int rank = comm->my_rank;
    int size = comm->local_group->grp_proc_count;
    nanompi_proc_t **grp_proc_pointers = comm->local_group->grp_proc_pointers;
    int i;
    struct sockaddr_in servaddr;

    // Make a client socket for every rank greater than mine. Last rank just returns
    // without doing anything
    for(i = rank + 1; i < size; i++) {
        if ((comm->socket_info.client_fds[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("client: socket creation error");
            status = MPI_ERR_OTHER;
            goto close_sock;
        }

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(grp_proc_pointers[i]->port);

        if (inet_pton(AF_INET, grp_proc_pointers[i]->hostname, &servaddr.sin_addr) <= 0) {
            perror("client: invalid address / address not supported (nick: use ip addresses in the hostfile until we add name resolution)");
            status = MPI_ERR_OTHER;
            goto close_sock;
        }

        // This client may have reached here before the server called accept(), so just keep trying
        while (connect(comm->socket_info.client_fds[i], (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0);
    }

exit:
    return status;
close_sock:
    for(i--; i >= rank + 1; i--) {
        close(comm->socket_info.client_fds[i]);
    }
    goto exit;
}

int nanompi_init_socket_backend(nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;
    int size = comm->local_group->grp_proc_count;

    comm->socket_info.client_fds = malloc(sizeof(int) * size);
    if (!comm->socket_info.client_fds) {
        printf("error mallocing client fds\n");
        status = MPI_ERR_OTHER;
    }

    status = init_clients(comm);
    if (!status) {
        goto free;
    }
    status = init_server(comm);
    if (!status) {
        goto free;
    }

    //sort the client_fds < rank so that they're in rank order--accept might not have filled the array in order

exit:
    return status;
free:
    free(comm->socket_info.client_fds);
}

int nanompi_free_socket_backend(nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;
    int size = comm->local_group->grp_proc_count;
    int rank = comm->my_rank;
    int i;

    for (i = 0; i < size; i++) {
        close(comm->socket_info.client_fds[i]);
    }
    free(comm->socket_info.client_fds);

    close(comm->socket_info.server_fd);

    return status;
}

int nanompi_socket_send(const void *buffer, size_t msg_size, int to_rank, nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;
    send(comm->socket_info.client_fds[to_rank], buffer, msg_size, 0);
    return status;
}

int nanompi_socket_recv(void *buffer, size_t msg_size, int from_rank, nanompi_communicator_t *comm)
{
    int status = MPI_SUCCESS;
    recv(comm->socket_info.client_fds[from_rank], buffer, msg_size, 0);
    return status;
}
