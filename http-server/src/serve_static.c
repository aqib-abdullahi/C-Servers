#include "http-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
/**
 * static_file - reads static file from a
 * file path
 * @file_path: path of file to be loaded
 * and read
 * return: characters loaded into buffer
 */

char* static_file(const char* file_path) {
    FILE *fd = fopen(file_path, "rb");
    if (!fd) {
        perror("Error opening File\n");
        fclose(fd);
        return NULL;
    }
    
    fseek(fd, 0, SEEK_END);
    long file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        perror("Failure to allocate memory\n");
        fclose(fd);
        return NULL;
    }

    ssize_t read_bytes = fread(buffer, 1, file_size, fd);
    if (read_bytes != file_size) {
        perror("error reading file\n");
        free(buffer);
        fclose(fd);
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(fd);
    return buffer;
}