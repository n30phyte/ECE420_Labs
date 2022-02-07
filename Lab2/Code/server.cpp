#include "server.h"

#include "common.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <sys/socket.h>

/**
 * Initializes server with arguments passed into main
 */
Server::Server(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <Size of array on server> <server ip> <server port>\n", argv[0]);
        exit(-1);
    }

    // Parse inputs
    table_size = strtol(argv[1], nullptr, 10);
    char *server_ip = argv[2];
    long server_port = strtol(argv[3], nullptr, 10);

    // Set up tables
    thread_table = new pthread_t[COM_NUM_REQUEST];
    data_table = new char *[table_size];
    memory_access_latency_table = new double[COM_NUM_REQUEST];
    // Initialize table with data
    for (auto i = 0; i < table_size; i++) {
        data_table[i] = new char[COM_BUFF_SIZE];
        sprintf(data_table[i], "String %d: the initial value", i);
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("Error creating socket: %d\n", errno);
        exit(-1);
    }

    sockaddr_in sock_var{};
    sock_var.sin_addr.s_addr = inet_addr(server_ip);
    sock_var.sin_port = server_port;
    sock_var.sin_family = AF_INET;

    int status = bind(socket_fd, (struct sockaddr *) &sock_var, sizeof(sock_var));
    if (status < 0) {
        printf("Error binding socket: %d\n", errno);
        exit(-1);
    }

#if COM_IS_VERBOSE == 1
    printf("Socket has been created\n");
#endif

    status = listen(socket_fd, 2000);
    if (status < 0) {
        printf("Error listening socket: %d\n", errno);
        exit(-1);
    }
}

[[noreturn]] void Server::run(void *(thread_function) (void *args)) const {
    while (true) {
        int i = 0;
        while (i < COM_NUM_REQUEST) {
            int client_fd = accept(socket_fd, nullptr, nullptr);
            if (client_fd < 0) {
                printf("Error accepting socket: %d\n", errno);
                continue;
            }
            auto params = new client_params;

            params->memory_access_latency_table = memory_access_latency_table;
            params->client_fd = client_fd;
            params->client_index = i;
            params->table = data_table;

            printf("Connected to client %d\n", client_fd);

            pthread_t thread_id;
            pthread_create(&thread_id, nullptr, thread_function, (void *) params);
            thread_table[i++] = thread_id;
        }
        for (auto j = 0; j < i; j++) {
            pthread_join(thread_table[j], nullptr);
        }
        saveTimes(memory_access_latency_table, COM_NUM_REQUEST);
    }
}

Server::~Server() {

#if COM_IS_VERBOSE
    for (long i = 0; i < table_size; i++) {
        printf("%s\n", data_table[i]);
    }
#endif

    close(socket_fd);

    free(thread_table);

    for (auto i = 0; i < table_size; i++) {
        free(data_table[i]);
    }
    free(data_table);
}
