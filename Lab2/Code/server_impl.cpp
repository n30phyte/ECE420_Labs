#include <cstdio>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "server.h"
#include "timer.h"

#if defined(SINGLE_MUTEX)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *single_mutex(void *args) {

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

        pthread_mutex_lock(&mutex);
#if COM_IS_VERBOSE
        printf("%d locked\n", client_fd);
#endif

        if (request.is_read) {
            getContent(response, request.pos, table);
        } else {
            setContent(request.msg, request.pos, table);
            getContent(response, request.pos, table);
        }
        write(client_fd, response, COM_BUFF_SIZE);

#if COM_IS_VERBOSE
        printf("%d unlocked\n", client_fd);
#endif
        pthread_mutex_unlock(&mutex);
    }
    GET_TIME(finish)
    elapsed = finish - start;
    params->memory_access_latency_table[params->client_index] = elapsed;

    close(client_fd);

    return nullptr;
}

#elif defined(MULTI_MUTEX)

pthread_mutex_t *mutex_table;

void *multi_mutex(void *args) {

    auto params = (client_params *) args;

    int client_fd = params->client_fd;
    char **table = params->table;
    double start, finish, elapsed;

    char msg[COM_BUFF_SIZE];
    read(client_fd, msg, COM_BUFF_SIZE);

#ifdef COM_IS_VERBOSE
        printf("reading from client: %s\n", msg);
#endif

    GET_TIME(start)
    {
        ClientRequest request;
        char response[COM_BUFF_SIZE];

        ParseMsg(msg, &request);

        pthread_mutex_lock(&mutex_table[request.pos]);
#ifdef COM_IS_VERBOSE
            printf("%d locked\n", client_fd);
#endif

        if (request.is_read) {
            getContent(response, request.pos, table);
        } else {
            setContent(request.msg, request.pos, table);
            getContent(response, request.pos, table);
        }
        write(client_fd, response, COM_BUFF_SIZE);

#ifdef COM_IS_VERBOSE
            printf("%d unlocked\n", client_fd);
#endif
        pthread_mutex_unlock(&mutex_table[request.pos]);
    }
    GET_TIME(finish)
    elapsed = finish - start;
    params->memory_access_latency_table[params->client_index] = elapsed;

    close(client_fd);

    return nullptr;
}

#elif defined(SINGLE_RWLOCK)

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *single_rwlock(void *args) {

    auto params = (client_params *) args;

    int client_fd = params->client_fd;
    char **table = params->table;
    double start, finish, elapsed;

    char msg[COM_BUFF_SIZE];
    read(client_fd, msg, COM_BUFF_SIZE);

#ifdef COM_IS_VERBOSE
        printf("reading from client: %s\n", msg);
#endif

    GET_TIME(start)
    {
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

#elif defined(MULTI_RWLOCK)

pthread_rwlock_t *rwlock_table;

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

#endif

int main(int argc, char *argv[]) {
    Server server(argc, argv);

#if defined(SINGLE_MUTEX)

    server.run(&single_mutex);

#elif defined(MULTI_MUTEX)

    mutex_table = (pthread_mutex_t *) malloc(server.table_size * sizeof(pthread_mutex_t));

    for (auto i = 0; i < server.table_size; i++) {
        pthread_mutex_init(&mutex_table[i], nullptr);
    }

    server.run(&multi_mutex);

#elif defined(SINGLE_RWLOCK)

    server.run(&single_rwlock);

#elif defined(MULTI_RWLOCK)

    rwlock_table = (pthread_rwlock_t *) malloc(server.table_size * sizeof(pthread_rwlock_t));

    for (auto i = 0; i < server.table_size; i++) {
        pthread_rwlock_init(&rwlock_table[i], nullptr);
    }

    server.run(&multi_rwlock);

#endif
}
