#include <stdio.h>
#include <stdlib.h>
#include "gps.h"
#include "uart.h"
#include "mraa.h"

int main(void) {
    // Open
    int avail;
    gps_init();
    char buffer[256];
    loc_t data;
    while (1) {
        avail = serial_avail();
	//printf("%d\n",avail);
       if(avail){gps_location(&data);
       //if(avail){
	   //serial_readln(buffer,256);
      //  printf("%s\n", buffer);
	//}
        printf("lat:%lf N lon:%lf E speed:%lf knots, alt: %lfm angel: %lf", data.latitude, data.longitude, data.speed, data.altitude, data.course);
      //printf("lat:%lf N lon:%lf E speed:%lf knots, alt: %lfm angel: %lf, mag: %lf %c\n", data.latitude, data.longitude, data.speed, data.altitude, data.course, data.magnetic, data.mag);
              }
       }

    return EXIT_SUCCESS;
}
