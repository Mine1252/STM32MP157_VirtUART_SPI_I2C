#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cJSON.h"




volatile sig_atomic_t stop = 0; // flag for stopping loop

void handle_sigint(int sig) {
    printf("Stopping\n");
    stop = 1; // tell main loop to stop
}


int main() {

    FILE* fp;
    char test[8192];

    signal(SIGINT, handle_sigint);

    system("echo bufSwapTesting_CM4.elf > /sys/class/remoteproc/remoteproc0/firmware");
    system("echo start > /sys/class/remoteproc/remoteproc0/state");

    sleep(3);

    fp = fopen("/dev/ttyRPMSG0", "r+");

    fprintf(fp, "Where is the voltage lebowski?\n");

    const cJSON *_data = NULL;

    int arraySize = 0;
    unsigned int sum = 0;
    double ave = 0.0;

    while (!stop) {
        fgets(test, 8192, fp);
        sum = 0;

        cJSON *json = cJSON_Parse(test);
        if (json == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                printf("Error: %s\n", error_ptr);
            }
            cJSON_Delete(json);
            // return 1;
            continue;
        }

        _data = cJSON_GetObjectItemCaseSensitive(json, "array");
        if (cJSON_IsArray(_data)) {
            
            arraySize = cJSON_GetArraySize(_data);
            
            for(int i = 0; i < arraySize; i++){
                sum = sum + cJSON_GetArrayItem(_data, i)->valueint;
            }

            ave = (double)sum / (double)arraySize;

            printf("Average: %0.2lf\n", ave);

        }
        // printf("%s\n", test);

        cJSON_Delete(json);

    }
    
    fclose(fp);

    system("echo stop > /sys/class/remoteproc/remoteproc0/state");

}