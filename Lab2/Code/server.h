#pragma once

#include <pthread.h>
#include <vector>

struct client_params {
    int client_fd;
    int client_index;
    char **table;
    double *memory_access_latency_table;
};

class Server {
public:
    char **data_table;
    double *memory_access_latency_table;
    pthread_t *thread_table;
    int socket_fd;
    long table_size;

    Server(int argc, char *argv[]);
    [[noreturn]] void run(void *(thread_function) (void *args)) const;
    ~Server();
};
