#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/select.h>
#include <time.h>

// Function declarations
void run_server_loop(int listen_fd);
void handle_new_connection(int listen_fd, fd_set *master_set, int *max_fd);
void handle_existing_client(int fd, fd_set *master_set);

#endif // EVENT_LOOP_H
