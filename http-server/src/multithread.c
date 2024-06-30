#include "http-server.h"
#include <stdio.h>
/**
 * multi_client - function handles multiple client
 * processes at once
 * return: void pointer
 */


void multi_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);
    client_process(client_socket);
    #ifdef _WIN32
    _endthread(); //For Windows
    #else
    pthread_exit(NULL); // For Unix-like
    #endif
    return;
}