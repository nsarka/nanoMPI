#ifndef NANOMPI_SOCKET_BACKEND_H
#define NANOMPI_SOCKET_BACKEND_H

typedef struct nanompi_communicator_t nanompi_communicator_t;

/*
 * Somebody has to be the client and somebody has to be the server
 * with sockets. So, decide who does what based on rank
 */
typedef struct nanompi_socket_info_t {
    int *server_fds; // accept lesser ranks' connections as a server
    int *client_fds; // connect to greater ranks as a client
} nanompi_socket_info_t;

int nanompi_init_socket_backend(nanompi_communicator_t *comm);
int nanompi_init_free_backend(nanompi_communicator_t *comm);

#endif
