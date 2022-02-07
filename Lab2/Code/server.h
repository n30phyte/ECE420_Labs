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
    Server(int argc, char *argv[]);
    void run(void *(thread_function) (void *args));
    ~Server();
    char **table;
    double *memory_access_latency_table;
    std::vector<pthread_t> threads;
    int socket_fd;
    long table_size;
};
