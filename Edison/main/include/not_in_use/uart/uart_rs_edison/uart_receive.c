#include <mraa.h>
#include <stdio.h>
#include <string.h>


int main(){	
	
	mraa_uart_context ed;
	ed = mraa_uart_init(0);
	mraa_uart_set_baudrate(ed, 38400);
	char buf[31];
	char flag[1];
	int i = 0;
	while(1) {
		mraa_uart_read(ed, flag, 1);
		if(flag[0]=='~'){
			buf[0] = '~';
			while(1){
				i++;
				mraa_uart_read(ed, buf+i, 1);
				if (buf[i] == '$'){
					buf[i+1] = '\0';
					break;
				}
			}
			i = 0;
			printf("%s\n", buf);
		}
	}
	return 0;
}
