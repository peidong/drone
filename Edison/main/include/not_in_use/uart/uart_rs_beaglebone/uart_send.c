#include <mraa.h>
#include <stdio.h>
#include "bbb/common.h"

int main(){	
	load_device_tree("ADAFRUIT-UART4");
	mraa_uart_context bbb;
	bbb = mraa_uart_init_raw("/dev/ttyO4");

	mraa_uart_set_baudrate(bbb, 38400);
	mraa_uart_set_mode(bbb, 8,MRAA_UART_PARITY_NONE , 1);
	char buf[31] = "~4121|p123.324321|n053.989876$";
    buf[30] = '\0';

    while (1) {
        mraa_uart_write(bbb, buf, 30);
        usleep(10000);
    }
    return 0;
}
