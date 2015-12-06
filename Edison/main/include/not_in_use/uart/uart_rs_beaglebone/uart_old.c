#include <mraa.h>
#include <stdio.h>
#include "bbb/common.h"

void bbbDecode(const char* mess)
{
	
}

int main(){	
	load_device_tree("ADAFRUIT-UART4");
	mraa_uart_context bbb;
	bbb = mraa_uart_init_raw("/dev/ttyO4");
	// bbb = mraa_uart_init(0);

	printf("%d\n", bbb);
	mraa_uart_set_baudrate(bbb, 38400);
	mraa_uart_set_mode(bbb, 8,MRAA_UART_PARITY_NONE , 1);
	char buf[20];
	char flag[1];
	int i = 0;
    while(1)
    {
			mraa_uart_read(bbb, flag, 1);	
			if(flag[0]=='~'){
				/*for(i = 0; i < 3; i++)	{*/
					/*mraa_uart_read(bbb, buf+i, 1);*/
                /*}*/
                buf[0] = '~';
                while(1){
                    i++;
                    mraa_uart_read(bbb, buf+i, 1);
                    if (buf[i] == '$'){
                        buf[i+1] = '\0';
                        break;
                    }
                }
                i = 0;

                bbbDecode(buf);

				// buf[5]=' ';

				printf("%s\n", buf);
			}

    }
}
