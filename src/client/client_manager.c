/**
 * @file client_manager.c
 * @brief HTTP Server - Client Connection Management Implementation
 * @version 1.0.0
 * @date 2025-06-07
 * @author David Dev (@DavidDevGt)
 * 
 * @description
 * Implementation of client connection lifecycle management for HTTP keep-alive
 * connections. Handles connection tracking, timeout management, and connection
 * pool maintenance for optimal server performance.
 * 
 * Features:
 * - Dynamic connection pool management
 * - Automatic timeout cleanup
 * - Request counting and limits
 * - Real-time connection statistics
 * 
 * @license MIT License
 */

#include "client_manager.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>

client_info_t clients[MAX_CLIENTS];
int client_count = 0;

void init_client_manager(void)
{
    memset(clients, 0, sizeof(clients));
    client_count = 0;
}

client_info_t* find_client(int fd)
{
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].fd == fd)
            return &clients[i];
    }
    return NULL;
}

void add_client(int fd)
{
    if (client_count < MAX_CLIENTS)
    {
        clients[client_count].fd = fd;
        clients[client_count].last_activity = time(NULL);
        clients[client_count].request_count = 0;
        clients[client_count].keep_alive = 1;
        client_count++;
        printf("Added client fd=%d, total clients: %d\n", fd, client_count);
    }
    else
    {
        printf("Max clients reached, cannot add fd=%d\n", fd);
    }
}

void remove_client(int fd)
{
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].fd == fd)
        {
            printf("Removing client fd=%d, requests served: %d\n", fd, clients[i].request_count);
            clients[i] = clients[client_count - 1];
            client_count--;
            printf("Total clients now: %d\n", client_count);
            break;
        }
    }
}

void cleanup_expired_connections(fd_set *master_set, int *max_fd)
{
    time_t current_time = time(NULL);
    int i = 0;
    
    while (i < client_count)
    {
        client_info_t *client = &clients[i];
        
        if (current_time - client->last_activity > KEEP_ALIVE_TIMEOUT)
        {
            printf("Closing expired connection fd=%d (inactive for %ld seconds)\n", 
                   client->fd, current_time - client->last_activity);
            close(client->fd);
            FD_CLR(client->fd, master_set);
            
            if (client->fd == *max_fd)
            {
                *max_fd = 0;
                for (int j = 0; j < FD_SETSIZE; j++)
                {
                    if (FD_ISSET(j, master_set) && j > *max_fd)
                        *max_fd = j;
                }
            }
            
            clients[i] = clients[client_count - 1];
            client_count--;
        }
        else
        {
            i++;
        }
    }
}

void print_connection_stats(void)
{
    time_t current_time = time(NULL);
    printf("=== Connection Statistics ===\n");
    printf("Active connections: %d/%d\n", client_count, MAX_CLIENTS);
    
    for (int i = 0; i < client_count; i++)
    {
        printf("Client fd=%d: %d requests, inactive for %ld seconds, keep_alive=%s\n",
               clients[i].fd, 
               clients[i].request_count,
               current_time - clients[i].last_activity,
               clients[i].keep_alive ? "yes" : "no");
    }
    printf("=============================\n");
}
