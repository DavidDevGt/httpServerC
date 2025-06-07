/**
 * @file event_loop.h
 * @brief HTTP Server Core - Event Loop Interface
 * @version 1.0.0
 * @date 2025-06-07
 * @author David Dev (@DavidDevGt)
 * 
 * @description
 * Event loop and I/O multiplexing interface for the HTTP server.
 * Handles connection management using select() system call for efficient
 * concurrent connection processing.
 * 
 * @license MIT License
 */

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/select.h>
#include <time.h>

void run_server_loop(int listen_fd);
void handle_new_connection(int listen_fd, fd_set *master_set, int *max_fd);
void handle_existing_client(int fd, fd_set *master_set);

#endif // EVENT_LOOP_H
