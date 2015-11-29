#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

int uart_setup(const char *dt)
{
    if (load_device_tree(dt)) {
        return 1;
    }

    return 0;
}

void uart_cleanup(void)
{
    unload_device_tree("ADAFRUIT-UART1");
    unload_device_tree("ADAFRUIT-UART2");
    unload_device_tree("ADAFRUIT-UART3");
    unload_device_tree("ADAFRUIT-UART4");
    unload_device_tree("ADAFRUIT-UART5");
}
