#include "http-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * client - handles client side process
 * return: nothing
 */


void client_process(int client_socket) {
    int sender;
    int read_bytes;
    char buffer[BUFFER_MAX];

    memset(buffer, 0, BUFFER_MAX);

    #ifdef _WIN32
    read_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    #else
    read_bytes = read(client_socket, buffer, sizeof(buffer) - 1);
    #endif

    if (read_bytes < 0) {
        printf("error reading request\n");
        #ifdef _WIN32
        shutdown(client_socket, 2);
        #else
        close(client_socket);
        #endif
        return;
    } else if (read_bytes == 0) {
        printf("client close connection\n");
    } else {
        buffer[read_bytes] = '\0';
        printf("read_bytes: %d\n", read_bytes);
        printf("Bytes read (on buffer): %d.\nBuffer: %s", read_bytes, buffer);
    }
    
    const char* response_header;
    response_header = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 12\r\n"
            "\r\n";

    const char* response_body;
    response_body = "Hello world!\n";

    int response_length = strlen(response_header) + strlen(response_body);
    char *response = (char *)malloc(response_length + 1);
    if (response == NULL) {
        perror("Failed to allocate memory for response");
    } else {
        strcpy(response, response_header);
        strcat(response, response_body);

        sender = send(client_socket, response, response_length, 0);
        if (sender < 0) {
            perror("Socket error");
        }
    // return;
    }
    #ifdef _WIN32            
    shutdown(client_socket, 2);
    #else
    close(client_socket);
    #endif
};