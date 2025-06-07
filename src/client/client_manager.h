#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <time.h>
#include <sys/select.h>

#define MAX_CLIENTS 100
#define KEEP_ALIVE_TIMEOUT 30  // seconds
#define MAX_REQUESTS_PER_CONNECTION 100

// Structure to track client connection state
typedef struct {
    int fd;
    time_t last_activity;
    int request_count;
    int keep_alive;
} client_info_t;

// Global variables (defined in client_manager.c)
extern client_info_t clients[MAX_CLIENTS];
extern int client_count;

// Client management functions
void init_client_manager(void);
client_info_t* find_client(int fd);
void add_client(int fd);
void remove_client(int fd);
void cleanup_expired_connections(fd_set *master_set, int *max_fd);
void print_connection_stats(void);

#endif // CLIENT_MANAGER_H
