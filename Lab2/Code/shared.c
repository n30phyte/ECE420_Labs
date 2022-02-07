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

    table = (char **) malloc(server->table_size * sizeof(char *));

    for (auto i = 0; i < server->table_size; i++) {
        table[i] = (char *) malloc(COM_BUFF_SIZE * sizeof(char));
        sprintf(table[i], "String %d: the initial value", i);
    }

    std::vector<pthread_t> threads;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sock_var;
    sock_var.sin_addr.s_addr = inet_addr(server->server_ip);
    sock_var.sin_port = server->server_port;
    sock_var.sin_family = AF_INET;

    int status = bind(socket_fd, (struct sockaddr *) &sock_var, sizeof(sock_var));
    if (status < 0) {
        printf("socket creation failed, errno: %d\n", status);
        close(socket_fd);
        return -1;
    }

    printf("socket has been created\n");
    status = listen(socket_fd, 2000);

    if (status < 0) {
        printf("listen failed, errno: %d\n", status);
        close(socket_fd);
        return -1;
    }

    while (threads.size() < COM_NUM_REQUEST) {
        int client_fd = accept(socket_fd, nullptr, nullptr);
        printf("Connected to client %d\n", client_fd);

        pthread_t thread_id;

        pthread_create(&thread_id, nullptr, thread_function, (void *) client_fd);
        threads.push_back(thread_id);
    }

    for (auto thread : threads) {
        pthread_join(thread, nullptr);
    }

    for (long i = 0; i < server->table_size; i++) {
        printf("%s\n", table[i]);
    }

    close(socket_fd);

    return 0;
}