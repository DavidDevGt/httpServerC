/**
 * @file client_manager.h
 * @brief HTTP Server - Client Connection Management Interface
 * @version 1.0.0
 * @date 2025-06-07
 * @author David Dev (@DavidDevGt)
 * 
 * @description
 * Client connection lifecycle management interface for keep-alive connections.
 * Tracks connection states, timeouts, and request counts for efficient
 * connection pool management.
 * 
 * Features:
 * - Keep-alive connection tracking
 * - Automatic timeout management
 * - Request counting per connection
 * - Connection statistics monitoring
 * 
 * @license MIT License
 */

#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <time.h>
#include <sys/select.h>

#define MAX_CLIENTS 100
#define KEEP_ALIVE_TIMEOUT 30
#define MAX_REQUESTS_PER_CONNECTION 100

typedef struct {
    int fd;
    time_t last_activity;
    int request_count;
    int keep_alive;
} client_info_t;

extern client_info_t clients[MAX_CLIENTS];
extern int client_count;

void init_client_manager(void);
client_info_t* find_client(int fd);
void add_client(int fd);
void remove_client(int fd);
void cleanup_expired_connections(fd_set *master_set, int *max_fd);
void print_connection_stats(void);

#endif // CLIENT_MANAGER_H
