#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Program Parameters */
#define COM_IS_VERBOSE 0 // 0 off; 1 on

// Do not change the following for your final testing and submitted version
#define COM_NUM_REQUEST 1000        // Number of total request \
                                    // For ease of implementation, prepare this many threads in server to handle the request
#define COM_BUFF_SIZE 100           // communication buffer size, which is the maximum size of the transmitted string
#define COM_CLIENT_THREAD_COUNT 100 // Number of threads in client, COM_NUM_REQUEST should be divisible by this Number
#define ART_DELAY 10                // artificial delay, in microseconds, for all R/W process

// Server utilities
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct {
    int pos;
    int is_read;
    char msg[COM_BUFF_SIZE];
} ClientRequest;

/**
* Parse the massage from client and get the control info and message info
*
* @param[in] msg message string with format "pos-is_read-XXXXXXX"
* @param[out] request previously allocated structure to store the parsed result
* @return
*/
int ParseMsg(char *msg, ClientRequest *request);

/**
* Function to set the server string array
*
* @param src source string to save
* @param pos position to be changed
* @param theArray server string array
*/
void setContent(char *src, int pos, char **theArray);

/**
 * Function to get the server string array
 * @param dst String buffer to store the server content
 * @param pos Position to get
 * @param theArray Server string array
 */
void getContent(char *dst, int pos, char **theArray);

/**
* Function to save the measured time
* @param time pointer to the array that store the time for each request
* @param length length of array \p time
*/
void saveTimes(const double *time, int length);

#ifdef __cplusplus
}
#endif