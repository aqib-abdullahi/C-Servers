#include "http-server.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

/**
 * main - function starts the http
 * server
 * Return: int 0
 */


int main() {
#ifdef _WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
#endif
    int server_socket;
    int server_sock_bind;
    int listener;
    #ifdef _WIN32
    char opt = 1;
    #else
    int opt = 1;
    #endif

    struct sockaddr_in server_address; 
    struct sockaddr_in client_address;
    socklen_t client_address_len;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error creating a server socket\n");
        exit(EXIT_FAILURE);
    }
    printf("socket created successfully\n");
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) {
        perror("Set socket options error");
        close(server_socket);
        exit(EXIT_FAILURE);
    }


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    server_sock_bind = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (server_sock_bind < 0) {
        printf("Error binding socket to address\n");
        shutdown(server_socket, 2);
        exit(EXIT_FAILURE);
    }
    printf("Server address bound to socket successfully\n");

    listener = listen(server_socket, 4);    /* Allow 4 connection */
    if (listener < 0) {
        printf("Error listenin to incoming connection\n");
        shutdown(server_socket, 2);
        exit(EXIT_FAILURE);
    }
    printf("server listening on port %d\n", PORT);

    for (;;) {
        client_address_len = sizeof(client_address);
        int *client_socket = malloc(sizeof(int));
        if (!client_socket) {
            perror("not able to allocate memory\n");
            // printf("failed to allocate memory");
            continue;
        }

        *client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (*client_socket < 0) {
            printf("Error accepting connection");
            free(client_socket);
            continue;
        }
        // For windows
        #ifdef _WIN32
        uintptr_t thread = _beginthread(multi_client, 0, client_socket);
        printf("\nbegin thread\n");
        if (thread == 0) {
            printf("failed to create a thread\n");
            free(client_socket);
        }
        #else
        pthread_t thread;
        if (pthread_create(&thread, NULL, (void *)multi_client, client_socket) != 0) {
            perror("Thread creation error");
            free(client_socket);
        }
        pthread_detach(thread);
        #endif
    }

    #ifdef _WIN32
    WSACleanup();
    #else
    close(server_socket);
    #endif

    return 0;
};