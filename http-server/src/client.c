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
    
    char method[16], path[256];
    sscanf(buffer, "%15s %255s", method, path);

    printf("Method: %s\n", method);
    printf("Path: %s\n", path);

    char content[BUFFER_MAX];
    const char* response;
    response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "\r\n%s";
    
    char complete_response[BUFFER_MAX];

    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            snprintf(content, BUFFER_MAX, 
                            "<html><body><h1>Hello world</h1></body></html>");
        } else
        {
            snprintf(content, BUFFER_MAX,
                            "<html><body><h1>Resource not found</h1></body></html>");
        }
    } else {
        response = 
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "\r\n%s";
        
        snprintf(content, BUFFER_MAX, 
                            "<html><body><h1>Method Not Allowed</h1></body></html>");
    }

    snprintf(complete_response, BUFFER_MAX, response, strlen(content), content);

    sender = send(client_socket, complete_response, strlen(complete_response), 0);
    if (sender < 0) {
        perror("Socket error");
    }

    #ifdef _WIN32            
    shutdown(client_socket, 2);
    #else
    close(client_socket);
    #endif
};