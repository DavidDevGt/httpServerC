#ifndef SERVER_H
#define SERVER_H

#include <sys/select.h>
#include <time.h>

#define PORT 6090
#define BACKLOG 10

// Function declarations
int create_listen_socket(int port);
void print_server_info(void);

#endif // SERVER_H
