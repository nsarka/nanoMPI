#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

#define NDEBUG // Comment for debug prints

void execute_command_on_host(const char *hostname, const char *cmd, int rank, int size) {
    char ssh_command[MAX_LINE_LENGTH] = {0};

    snprintf(ssh_command, sizeof(ssh_command), "ssh %s 'export NANOMPI_WORLD_RANK=%d ; export NANOMPI_WORLD_SIZE=%d ; export LD_LIBRARY_PATH=%s ; cd %s ; %s'", hostname, rank, size, getenv("LD_LIBRARY_PATH"), getenv("PWD"), cmd);

#ifndef NDEBUG
    printf("%s\n", ssh_command);
#endif

    if (system(ssh_command) == -1) {
        perror("system");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname_file> <command>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH] = {0};
    int rank = 0;
    int size = 0;

    // Count how many ranks total
    while (fgets(line, sizeof(line), file)) {
        size++;
    }
    fseek(file, 0, SEEK_SET);

    while (fgets(line, sizeof(line), file)) {
        // Remove newline character from the end of the line
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Skip empty lines and comment lines
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        // Execute the command on the hostname
        execute_command_on_host(line, argv[2], rank, size);

        rank++;
    }

    fclose(file);
    return EXIT_SUCCESS;
}