#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#include "event_loop.h"
#include "../client/client_manager.h"
#include "../http/http_handler.h"

void handle_new_connection(int listen_fd, fd_set *master_set, int *max_fd)
{
    int client_fd = accept(listen_fd, NULL, NULL);
    if (client_fd >= 0)
    {
        if (client_count >= MAX_CLIENTS)
        {
            printf("Max clients reached, rejecting connection fd=%d\n", client_fd);
            const char *response = "HTTP/1.1 503 Service Unavailable\r\n"
                                 "Content-Length: 26\r\n"
                                 "Connection: close\r\n\r\n"
                                 "<h1>Server Overloaded</h1>";
            send(client_fd, response, strlen(response), 0);
            close(client_fd);
        }
        else
        {
            FD_SET(client_fd, master_set);
            if (client_fd > *max_fd)
                *max_fd = client_fd;
            add_client(client_fd);
        }
    }
}

void handle_existing_client(int fd, fd_set *master_set)
{
    if (handle_client(fd) < 0)
    {
        close(fd);
        FD_CLR(fd, master_set);
        remove_client(fd);
    }
}

void run_server_loop(int listen_fd)
{
    fd_set master_set, read_set;
    FD_ZERO(&master_set);
    FD_SET(listen_fd, &master_set);

    int max_fd = listen_fd;
    struct timeval timeout;
    time_t last_stats_print = time(NULL);
    
    while (1)
    {
        read_set = master_set;
        
        // Set timeout for select to enable periodic cleanup
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int ready = select(max_fd + 1, &read_set, NULL, NULL, &timeout);
        if (ready < 0)
        {
            perror("select");
            exit(1);
        }
        
        // Cleanup expired connections periodically
        cleanup_expired_connections(&master_set, &max_fd);
        
        // Print connection statistics every 30 seconds
        time_t current_time = time(NULL);
        if (current_time - last_stats_print >= 30)
        {
            print_connection_stats();
            last_stats_print = current_time;
        }

        for (int fd = 0; fd <= max_fd && ready > 0; ++fd)
        {
            if (!FD_ISSET(fd, &read_set))
                continue;
                
            ready--;
            
            if (fd == listen_fd)
            {
                // new client connection
                handle_new_connection(listen_fd, &master_set, &max_fd);
            }
            else
            {
                // Handle existing client
                handle_existing_client(fd, &master_set);
            }
        }
    }
}
