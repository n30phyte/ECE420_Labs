#include "server.h"

#include "common.h"

#include <arpa/inet.h>
#include <cstdio>
#include <sys/socket.h>

Server::Server(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "usage: %s <Size of theArray_ on server> <server ip> <server port>\n", argv[0]);
        exit(0);
    }

    table_size = strtol(argv[1], nullptr, 10);
    char *server_ip = argv[2];
    long server_port = strtol(argv[3], nullptr, 10);

    table = (char **) malloc(table_size * sizeof(char *));

    for (auto i = 0; i < table_size; i++) {
        table[i] = (char *) malloc(COM_BUFF_SIZE * sizeof(char));
        sprintf(table[i], "String %d: the initial value", i);
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sock_var;
    sock_var.sin_addr.s_addr = inet_addr(server_ip);
    sock_var.sin_port = server_port;
    sock_var.sin_family = AF_INET;

    int status = bind(socket_fd, (struct sockaddr *) &sock_var, sizeof(sock_var));
    if (status < 0) {
        printf("socket creation failed, errno: %d\n", status);
        close(socket_fd);
    }

    printf("socket has been created\n");
    status = listen(socket_fd, 2000);

    if (status < 0) {
        printf("listen failed, errno: %d\n", status);
        close(socket_fd);
    }
}

void Server::run(void *(thread_function) (void *args)) {
    while (true) {
//        while (threads.size() < COM_NUM_REQUEST) {
            int client_fd = accept(socket_fd, nullptr, nullptr);
            auto params = (client_params *) malloc(sizeof(client_params));

            params->client_fd = client_fd;
            params->table = table;

            printf("Connected to client %d\n", client_fd);
            printf("Attempt Number: %zu\n", threads.size());

            pthread_t thread_id;
            pthread_create(&thread_id, nullptr, thread_function, (void *) params);
            threads.push_back(thread_id);
//        }
    }
}

Server::~Server() {
    for (auto thread : threads) {
        pthread_join(thread, nullptr);
    }

    if (COM_IS_VERBOSE) {
        for (long i = 0; i < table_size; i++) {
            printf("%s\n", table[i]);
        }
    }

    close(socket_fd);
}
