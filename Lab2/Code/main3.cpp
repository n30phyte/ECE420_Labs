#include <cstdio>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "server.h"
#include "timer.h"

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

/**
 * Use an RWLock to handle locking for entire table. 
 */
void *single_rwlock(void *args) {

    auto params = (client_params *) args;

    int client_fd = params->client_fd;
    char **table = params->table;
    double start, finish, elapsed;

    char msg[COM_BUFF_SIZE];
    read(client_fd, msg, COM_BUFF_SIZE);

#if COM_IS_VERBOSE
    printf("reading from client: %s\n", msg);
#endif

    GET_TIME(start) {
        ClientRequest request;
        char response[COM_BUFF_SIZE];

        ParseMsg(msg, &request);

        if (request.is_read) {
            pthread_rwlock_rdlock(&rwlock);
            getContent(response, request.pos, table);
            pthread_rwlock_unlock(&rwlock);
        } else {
            pthread_rwlock_wrlock(&rwlock);
            setContent(request.msg, request.pos, table);
            getContent(response, request.pos, table);
            pthread_rwlock_unlock(&rwlock);
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

    server.run(&single_rwlock);
}
