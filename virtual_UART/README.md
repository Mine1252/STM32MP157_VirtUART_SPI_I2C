## STM32MP157 Uart Data Transmission Through JSON

A working example is contained within this repo

### M4 Core
#### Virtual UART
First Include:
```
#include "virt_uart.h"
#include "openamp_log.h"
```
Then initialize the variable
```
VIRT_UART_HandleTypeDef huart0;
```

Next initialize the Virtual UART channel. This needs to be in the main function:
```
  log_info("Virtual UART0 OpenAMP-rpmsg channel creation\r\n");
  if (VIRT_UART_Init(&huart0) != VIRT_UART_OK) {
    log_err("VIRT_UART_Init UART0 failed.\r\n");
    Error_Handler();
  }

  /*Need to register callback for message reception by channels*/
  if(VIRT_UART_RegisterCallback(&huart0, VIRT_UART_RXCPLT_CB_ID, VIRT_UART0_RxCpltCallback) != VIRT_UART_OK)
  {
   Error_Handler();
  }
```

Next in the while loop place this line before any data is transmitted. 
```
OPENAMP_check_for_message();
```

NOTE: To allow the data transmission the A7 core must finish the link by sending a message to the new tty device that appeared. This message can be anything. Without this the M4 core will not send any data to the new tty device.

Finally to send data use this function:
```
VIRT_UART_Transmit(&huart0, (uint8_t*)"Some String", string len: int);
```


#### JSON Formatting
First include
```
#include "cJSON.h"
#include "string.h"
```

Then initialize a json variable. This needs to be in the while loop
```
cJSON *JSONVariableName = cJSON_CreateObject();
```

To add a number to the json object use this function:
```
cJSON_AddNumberToObject(JSONVariableName, "item name", value);
```

To initialize and add an array to the JSON object use these functions:
```
cJSON *arrayVariableName = cJSON_CreateIntArray((const int *)list, 1024);

cJSON_AddItemToObject(JSONVariableName, "item name", arrayVariableName);
```

To get a variable containing a string version of this JSON object use this function
```
char *json_str = cJSON_PrintUnformatted(root);
```

#### Sending the JSON Packet
If the JSON packet is less than 512 characters long it can be sent with no problem. If it is 512 characters ot longer it needs to be broken into chunks. An example function that can break a long string into arbitrary chink sizes and transmit them over the virtual UART is the following:
```
#define CHUNK_SIZE 256 // Can be any number < 512 for virtual UART

void process_large_string(const char *data) {
    size_t total_len = strlen(data);
    size_t offset = 0;

    while (offset < total_len) {
        size_t remaining = total_len - offset;
        size_t this_len = remaining > CHUNK_SIZE ? CHUNK_SIZE : remaining;

        VIRT_UART_Transmit(&huart0, (uint8_t*)data + offset, this_len);

        offset += this_len;
    }
    VIRT_UART_Transmit(&huart0, (uint8_t*)"\n", 1);
}
```

### A7 Core

The A7 core has the following tasks
- Loading the firmware into the M4 core
- Starting the M4 core
- Completing the virtual UART transmission by sending a message
- Interpreting the data  recieved from the virtual UART channel
- Stopping the M4 core

The following libraries are necessary:
```
#include <signal.h> // To catch the Ctrl + c to gracefully stop the program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // sleep function
#include <unistd.h> 
#include "cJSON.h" // To interpret the json packets
```

To load the firmware on the the M4 core:
```
system("echo firmwareName.elf > /sys/class/remoteproc/remoteproc0/firmware");
```


#### Starting M4 Core
To start the M4 core:
```
system("echo start > /sys/class/remoteproc/remoteproc0/state");

sleep(3); // Recomended to give time for the M4 core to start. Can probably be lower
```

#### UART Link and Packet Grabbing
To finish the virtual UART link:
```
FILE* fp;

fp = fopen("/dev/ttyRPMSG0", "r+");

fprintf(fp, "Any message will do\n");
```

To read the full packet from the virtual UART channel use this:
```
char test[8192]; // This array should be larger than whatever packet you intend to make
fgets(test, 8192, fp);
```

#### JSON Packet Interpretation and Processing
To interpret the packet use the following:
```
const cJSON *_data = NULL;

_data = cJSON_GetObjectItemCaseSensitive(json, "array");

if (cJSON_IsArray(_data)) {
    
    arraySize = cJSON_GetArraySize(_data);
    
    for(int i = 0; i < arraySize; i++){
        sum = sum + cJSON_GetArrayItem(_data, i)->valueint;
    }

    ave = (double)sum / (double)arraySize;

    printf("Average: %0.2lf\n", ave);

}

cJSON_Delete(json); // Clear the memory for a new packet

```

#### Stopping M4 Core
To Stop the M4 core:
```
system("echo stop > /sys/class/remoteproc/remoteproc0/state");
```