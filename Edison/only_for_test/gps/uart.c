#include <stdio.h>
//! [Interesting]
#include "mraa.h"
//#include "uart.h"
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

mraa_uart_context uart;

void serial_readln(char *buffer, int len)
{
	//char c;
	char *b = buffer;
  int length = len;
	int rx_length = -1;
	mraa_uart_read(uart, b, length);
		/*if(rx_length <= 0){
			sleep(1);
	      		}
		else{
		if(c == '\n'){
			*b++ = '\0';
			break;
			}
		*b++ = c;*/
}

void serial_println(const char *line, int len){
  if(uart!=NULL){
  char *cpstr = (char *)malloc((len+1) *sizeof(char));
  
  strcpy(cpstr,line);
  cpstr[len-1] ='\r';
  cpstr[len] = '\n';
  
  int count = mraa_uart_write(uart, cpstr, len+1);
  if (count < 0){}
  free(cpstr);
  }

}

void serial_init(void)
{//mraa_uart_context uart;
 uart = mraa_uart_init(0);
}
 
void serial_close(void){
mraa_uart_stop(uart);
mraa_deinit();}

int serial_avail(void)
{
  int k =-1;
  k = mraa_uart_data_available(uart, 0);
  return k;

}
/*
int main(int argc, char** argv)
{   int read;
    mraa_uart_context uart;
    uart = mraa_uart_init(0);
    //mraa_uart_init_raw("/dev/ttyGS0");
    mraa_uart_set_baudrate(uart, 9600);
    mraa_uart_set_mode(uart, 8, MRAA_UART_PARITY_NONE , 1);
    //printf("hello \n");
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
         avail= mraa_uart_data_available(uart, 0);
       printf("%d",avail);
        // printf("%i\n", avail);
    //read = mraa_uart_read(uart, buffer, sizeof(buffer));
     //  printf("%s\n",buffer);
       // printf("%i\n",read);
      //  printf("end:\n");
       }   
    mraa_uart_stop(uart);

    mraa_deinit();

    return EXIT_SUCCESS;
}
*/