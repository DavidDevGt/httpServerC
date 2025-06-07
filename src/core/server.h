/**
 * @file server.h
 * @brief HTTP Server Core - Server Management Interface
 * @version 1.0.0
 * @date 2025-06-07
 * @author David Dev (@DavidDevGt)
 * 
 * @description
 * Core server functionality for HTTP server implementation.
 * Handles socket creation, binding, and basic server configuration.
 * 
 * @license MIT License
 */

#ifndef SERVER_H
#define SERVER_H

#include <sys/select.h>
#include <time.h>

#define PORT 6090
#define BACKLOG 10

int create_listen_socket(int port);
void print_server_info(void);

#endif // SERVER_H
