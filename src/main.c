#include <stdio.h>
#include <stdlib.h>

#include "core/server.h"
#include "core/event_loop.h"
#include "client/client_manager.h"

int main()
{
    // Initialize client manager
    init_client_manager();
    
    // Create and configure the listening socket
    int listen_fd = create_listen_socket(PORT);
    
    // Print server information
    print_server_info();
    
    // Run the main server event loop
    run_server_loop(listen_fd);
    
    return 0;
}