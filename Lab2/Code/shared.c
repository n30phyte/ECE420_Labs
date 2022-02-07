#pragma once

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <vector>

#include "common.h"

typedef struct {
    long table_size;
    char *server_ip;
    long server_port;
} Server;

Server get_server(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <Size of theArray_ on server> <server ip> <server port>\n", argv[0]);
        exit(0);
    }
    Server server;
    server.table_size = strtol(argv[1], nullptr, 10);
    server.server_ip = argv[2];
    server.server_port = strtol(argv[3], nullptr, 10);
    return server;
}

int setup_thread_server(Server *server, char **table, void *(thread_function) (void *args)) {



}