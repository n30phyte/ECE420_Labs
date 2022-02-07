#include "common.h"

/**
* Parse the massage from client and get the control info and message info
*
* @param msg message string with format "pos-is_read-XXXXXXX"
* @param[out] rqst previously allocated structure to store the parsed result
* @return
*/
int ParseMsg(char *msg, ClientRequest *rqst) {
   char *token;
   token = strsep(&msg, "-");
   rqst->pos = atoi(token);
   token = strsep(&msg, "-");
   rqst->is_read = atoi(token);
   token = strsep(&msg, "-");
   memcpy(rqst->msg, token, strlen(token) + 1);
   return 0;
}

/**
* Function to set the server string array
*
* @param src source string to save
* @param pos position to be changed
* @param theArray server string array
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

// Function to get the server string array
// Input: dst: dst string buffer to get the server content
//        pos: position to be get
//        theArray: server string array
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
void saveTimes(double *time, int length) {
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