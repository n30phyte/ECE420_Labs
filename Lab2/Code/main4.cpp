#include <cstdio>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "server.h"
#include "timer.h"

pthread_rwlock_t *rwlock_table;

/**
 * Use an RWLock to handle locking for each entry in the table. 
 */
void *multi_rwlock(void *args) {

    auto params = (client_params *) args;

    int client_fd = params->client_fd;
    char **table = params->table;
    double start, finish, elapsed;

    char msg[COM_BUFF_SIZE];
    read(client_fd, msg, COM_BUFF_SIZE);

#ifdef COM_IS_VERBOSE
    printf("reading from client: %s\n", msg);
#endif

    GET_TIME(start) {
        ClientRequest request;
        char response[COM_BUFF_SIZE];

        ParseMsg(msg, &request);

        if (request.is_read) {
            pthread_rwlock_rdlock(&rwlock_table[request.pos]);
            getContent(response, request.pos, table);
            pthread_rwlock_unlock(&rwlock_table[request.pos]);
        } else {
            pthread_rwlock_wrlock(&rwlock_table[request.pos]);
            setContent(request.msg, request.pos, table);
            getContent(response, request.pos, table);
            pthread_rwlock_unlock(&rwlock_table[request.pos]);
        }
        write(client_fd, response, COM_BUFF_SIZE);
    }
    GET_TIME(finish)
    elapsed = finish - start;
    params->memory_access_latency_table[params->client_index] = elapsed;

    close(client_fd);

    return nullptr;
}

int main(int argc, char *argv[]) {
    Server server(argc, argv);

    rwlock_table = (pthread_rwlock_t *) malloc(server.table_size * sizeof(pthread_rwlock_t));

    for (auto i = 0; i < server.table_size; i++) {
        pthread_rwlock_init(&rwlock_table[i], nullptr);
    }

    server.run(&multi_rwlock);
}
