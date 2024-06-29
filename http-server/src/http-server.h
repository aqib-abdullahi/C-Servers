#ifdef _WIN32
// #define _WIN32
#include <winsock2.h>
#include <ws2spi.h>
#include <ws2tcpip.h>
#endif

#define PORT 8080
#define BUFFER_MAX 1024

void client_process(int client_socket);
