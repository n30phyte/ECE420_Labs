#include <cstdio>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

#include "server.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

        pthread_mutex_lock(&mutex);
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
        pthread_mutex_unlock(&mutex);
    }
    // END DO THINGS

    close(client_fd);

    return nullptr;
}

int main(int argc, char *argv[]) {
    Server server(argc, argv);

    server.run(&handle_client);
}
