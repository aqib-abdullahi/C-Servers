#ifdef _WIN32
#include <winsock2.h>
#include <ws2spi.h>
#include <ws2tcpip.h>
#include <process.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h
#endif

#define PORT 8080
#define BUFFER_MAX 1024

void client_process(int client_socket);
void multi_client(void* arg);