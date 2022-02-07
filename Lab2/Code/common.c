#include "common.h"

/**
* Parse the massage from client and get the control info and message info
*
* @param[in] msg message string with format "pos-is_read-XXXXXXX"
* @param[out] request previously allocated structure to store the parsed result
* @return
*/
int ParseMsg(char *msg, ClientRequest *request) {
    char *token;
    token = strsep(&msg, "-");
    request->pos = atoi(token);
    token = strsep(&msg, "-");
    request->is_read = atoi(token);
    token = strsep(&msg, "-");
    memcpy(request->msg, token, strlen(token) + 1);
    return 0;
}

/**
* Function to set the server string array
*
* @param src[in] source string to save
* @param pos[in] position to be changed
* @param theArray[in] server string array
*/
void setContent(char *src, int pos, char **theArray) {
    int i;
    char *dst = theArray[pos];
    int length = MIN(strlen(src) + 1, COM_BUFF_SIZE);
    usleep(ART_DELAY);
    for (i = 0; i < length; ++i) {
        dst[i] = src[i];
    }
}

/**
 * Function to get the server string array
 * @param dst[out] String buffer to store the server content
 * @param pos[in] Position to get
 * @param theArray[in] Server string array
 */
void getContent(char *dst, int pos, char **theArray) {
    int i;
    char *src = theArray[pos];
    int length = MIN(strlen(src) + 1, COM_BUFF_SIZE);
    usleep(ART_DELAY);
    for (i = length - 1; i >= 0; --i) {
        dst[i] = src[i];
    }
}

/**
* Function to save the measured time
* @param time pointer to the array that store the time for each request
* @param length length of array \p time
*/
void saveTimes(const double *time, int length) {
    FILE *op;
    int i;
    double elapsed_time = 0;
    for (i = 0; i < length; ++i) {
        elapsed_time += time[i];
    }
    elapsed_time /= length;
    if ((op = fopen("server_output_time_aggregated", "a+")) == NULL) {
        printf("Error opening the output file: server_output_time_aggregated.\n");
        exit(1);
    }
    fprintf(op, "%e\n", elapsed_time);
    fclose(op);
}