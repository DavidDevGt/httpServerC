#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "server.h"
#include "../client/client_manager.h"

int create_listen_socket(int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    // reuse address
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr =
        {
            .sin_family = AF_INET,               // IPv4
            .sin_port = htons(port),             // port
            .sin_addr.s_addr = htonl(INADDR_ANY) // any address
        };

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    // listening all for incoming clients
    // backlog max 10 connections
    if (listen(sockfd, BACKLOG) < 0)
    {
        perror("listen");
        exit(1);
    }

    return sockfd;
}

void print_server_info(void)
{
    printf("HTTP Server started on port %d with keep-alive support\n", PORT);
    printf("Keep-alive timeout: %d seconds\n", KEEP_ALIVE_TIMEOUT);
    printf("Max requests per connection: %d\n", MAX_REQUESTS_PER_CONNECTION);
    printf("Max concurrent clients: %d\n", MAX_CLIENTS);
}
