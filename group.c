#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "group.h"
#include "mpi.h"

int nanompi_init_group(nanompi_group_t *group, int rank, int world_size, char *hostfile)
{
    int i = 0, status = MPI_SUCCESS;
    char line[MAX_HOSTNAME_LENGTH] = {0};
    FILE *fp;

    fp = fopen(hostfile, "r");
    if (fp == NULL) {
        perror("fopen");
        status = MPI_ERR_OTHER;
        goto exit;
    }

    group->grp_my_rank = rank;
    group->grp_proc_count = world_size;

    group->grp_proc_pointers = malloc(sizeof(nanompi_proc_t *) * world_size);
    if (!group->grp_proc_pointers) {
        printf("Error: OOM allocating group->grp_proc_pointers\n");
        goto close;
    }

    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Skip empty lines and comment lines
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        group->grp_proc_pointers[i] = malloc(sizeof(nanompi_proc_t));
        if (!group->grp_proc_pointers[i]) {
            printf("Error: OOM allocating nanompi_proc_t\n");
            status = MPI_ERR_OTHER;
            goto free_hostnames;
        }
        memcpy(&group->grp_proc_pointers[i]->hostname, line, MAX_HOSTNAME_LENGTH);
        group->grp_proc_pointers[i]->port = 8000 + i;
        i++;
    }

    if (i != world_size) {
        printf("Error: expected %d procs in hostfile, got %d\n", world_size, i);
        status = MPI_ERR_OTHER;
        goto free_hostnames;
    }

close:
    fclose(fp);
exit:
    return status;
free_hostnames:
    for(; i > 0; i--) {
        free(group->grp_proc_pointers[i - 1]);
    }
free:
    free(group->grp_proc_pointers);
    goto close;
}

int nanompi_free_group(nanompi_group_t *group)
{
    int status = MPI_SUCCESS;
    int i;

    for(i = 0; i < group->grp_proc_count; i++) {
        free(group->grp_proc_pointers[i]);
    }

    free(group->grp_proc_pointers);
    free(group);
}
