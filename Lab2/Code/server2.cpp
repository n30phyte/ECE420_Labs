#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "common.h"

char **table;
pthread_mutex_t *mutex_table;

void *handle_client(void *args) {
    int client_fd = (long) args;
    char msg[COM_BUFF_SIZE];
    read(client_fd, msg, COM_BUFF_SIZE);

    if (COM_IS_VERBOSE) {
        printf("reading from client: %s\n", msg);
    }

    // DO THINGS
    {
        ClientRequest request;
        char response[COM_BUFF_SIZE];

        ParseMsg(msg, &request);

        pthread_mutex_lock(&mutex_table[request.pos]);
        if (COM_IS_VERBOSE) {
            printf("%d locked\n", client_fd);
        }

        if (request.is_read) {
            getContent(response, request.pos, table);
        } else {
            setContent(request.msg, request.pos, table);
            getContent(response, request.pos, table);
        }
        write(client_fd, response, COM_BUFF_SIZE);

        if (COM_IS_VERBOSE) {
            printf("%d unlocked\n", client_fd);
        }
        pthread_mutex_unlock(&mutex_table[request.pos]);
    }
    // END DO THINGS

    close(client_fd);

    return nullptr;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "usage: %s <Size of array> <server ip> <server port>\n", argv[0]);
        exit(0);
    }

    long table_size = strtol(argv[1], nullptr, 10);
    char *server_ip = argv[2];
    long server_port = strtol(argv[3], nullptr, 10);

    table = (char **) malloc(table_size * sizeof(char *));
    mutex_table = (pthread_mutex_t *) malloc(table_size * sizeof(pthread_mutex_t));

    for (auto i = 0; i < table_size; i++) {
        table[i] = (char *) malloc(COM_BUFF_SIZE * sizeof(char));
        sprintf(table[i], "String %d: the initial value", i);
        pthread_mutex_init(&mutex_table[i], nullptr);
    }

    std::vector<pthread_t> threads;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sock_var;
    sock_var.sin_addr.s_addr = inet_addr(server_ip);
    sock_var.sin_port = server_port;
    sock_var.sin_family = AF_INET;

    int status = bind(socket_fd, (struct sockaddr *) &sock_var, sizeof(sock_var));
    if (status < 0) {
        printf("socket creation failed\n");
        close(socket_fd);
        return -1;
    }

    printf("socket has been created\n");
    status = listen(socket_fd, 2000);

    if (status < 0) {
        printf("listen failed\n");
        close(socket_fd);
        return -1;
    }

    while (threads.size() < COM_NUM_REQUEST) {
        int client_fd = accept(socket_fd, nullptr, nullptr);
        printf("Connected to client %d\n", client_fd);

        pthread_t thread_id;

        pthread_create(&thread_id, nullptr, handle_client, (void *) client_fd);
        threads.push_back(thread_id);
    }

    for (auto thread : threads) {
        pthread_join(thread, nullptr);
    }

    for (long i = 0; i < table_size; i++) {
        printf("%s\n", table[i]);
    }

    close(socket_fd);

    return 0;
}
