/**
 * @file main.c
 * @brief HTTP Server C - Main Entry Point
 * @version 1.0.0
 * @date 2025-06-07
 * @author David Dev (@DavidDevGt)
 * 
 * @description
 * Main entry point for the HTTP server implementation with keep-alive support.
 * Initializes the server components and starts the main event loop.
 * 
 * Features:
 * - HTTP/1.1 with keep-alive connections
 * - Select-based I/O multiplexing
 * - Concurrent connection handling
 * - Real-time connection monitoring
 * 
 * @license MIT License
 */

#include <stdio.h>
#include <stdlib.h>

#include "core/server.h"
#include "core/event_loop.h"
#include "client/client_manager.h"

int main()
{
    init_client_manager();
    int listen_fd = create_listen_socket(PORT);
    print_server_info();
    run_server_loop(listen_fd);
    
    return 0;
}