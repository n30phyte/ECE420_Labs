#include <cstdio>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

#include "server.h"

pthread_mutex_t *mutex_table;

void *handle_client(void *args) {

    auto params = (client_params *) args;

    int client_fd = params->client_fd;
    char **table = params->table;

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
    Server server(argc, argv);

    mutex_table = (pthread_mutex_t *) malloc(server.table_size * sizeof(pthread_mutex_t));

    for (auto i = 0; i < server.table_size; i++) {
        pthread_mutex_init(&mutex_table[i], nullptr);
    }
    server.run(&handle_client);
}
