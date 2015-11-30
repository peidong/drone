#include <stdio.h>
//! [Interesting]
#include <mraa.h>
//#include "uart.h"
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

int main(int argc, char** argv)
{   mraa_init();
    int read;
    mraa_uart_context uart;
    uart = mraa_uart_init(0);
  // mraa_uart_init_raw("/dev/ttyGS0");
    mraa_uart_set_baudrate(uart, 9600);
    mraa_uart_set_mode(uart, 8, MRAA_UART_PARITY_NONE , 1);
    printf("hello \n");
    if (uart == NULL) {
        fprintf(stderr, "UART failed to setup\n");
        return EXIT_FAILURE;
    }

    char buffer[100];
    
   // printf("%s\n",buffer);
    // mraa_uart_write(uart, buffer, sizeof(buffer));
   while(1){ 
     // printf("start\n");
        int avail;
        avail= mraa_uart_data_available(uart,0);
       printf("%d",avail);
        // printf("%i\n", avail);
    //read = mraa_uart_read(uart, buffer, sizeof(buffer));
     //  printf("%s\n",buffer);
       // printf("%d\n",read);
      //  printf("end:\n");
       }   
    mraa_uart_stop(uart);

    mraa_deinit();

    return EXIT_SUCCESS;
 }
