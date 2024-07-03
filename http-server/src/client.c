#include "http-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <shlwapi.h>

/**
 * client_process - handles client side process
 * @client_socket - file description
 * return: nothing
 */


void client_process(int client_socket) {
    int sender;
    ssize_t read_bytes;
    char buffer[BUFFER_MAX];

    printf("\n started client process\n");
    read_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    printf("size of read bytes buffer: %d\n", read_bytes);
    if (read_bytes < 0) {
        perror("Error reading request.\n");
        #ifdef _WIN32
        shutdown(client_socket, 2);
        #else
        close(client_socket);
        #endif
        return;
    }

    buffer[read_bytes] = '\0';
    
    char method[16], path[256];
    sscanf(buffer, "%15s %255s", method, path);

    printf("Method: %s\n", method);
    printf("Path: %s\n", path);

    char* response_file = NULL;
    char* complete_response = NULL;
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            response_file = static_file("./hello.html");
            const char *response_ok_template = "HTTP/1.1 200 OK\r\n"
                                       "Content-Type: text/html\r\n"
                                       "Content-Length: %d\r\n"
                                       "\r\n%s";
            if (response_file) {
                ssize_t response_file_size = snprintf(NULL, 0, response_ok_template, (int)strlen(response_file), response_file) + 1;
                complete_response = malloc(response_file_size);
                snprintf(complete_response, response_file_size, response_ok_template, (int)strlen(response_file), response_file);
            } else {
                const char *hello_world = "<html><body><h1>Hello world</h1></body></html>";
                snprintf(complete_response,  strlen(response_ok_template) + strlen(hello_world), response_ok_template, (int)strlen(hello_world), hello_world);
            }
        } else {
            const char *response_not_found_template = "HTTP/1.1 404 Not Found\r\n"
                                              "Content-Type: text/html\r\n"
                                              "Content-Length: 53\r\n"
                                              "\r\n<html><body><h1>Resource not found</h1></body></html>";
            complete_response = strdup(response_not_found_template);
        }    
    } else {
        const char *response_not_allowed_template = "HTTP/1.1 405 Method Not Allowed\r\n"
                                       "Content-Type: text/html\r\n"
                                       "Content-Length: 51\r\n"
                                       "\r\n<html><body><h1>Method Not Allowed</h1></body></html>";
        complete_response = strdup(response_not_allowed_template);
    }

    if (complete_response) {
        sender = send(client_socket, complete_response, strlen(complete_response), 0);
        free(complete_response);
    }

    if (response_file) {
        free(response_file);
    }

    #ifdef _WIN32            
    shutdown(client_socket, 2);
    #else
    close(client_socket);
    #endif
};