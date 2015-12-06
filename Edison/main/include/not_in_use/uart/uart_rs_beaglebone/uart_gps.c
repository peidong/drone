#include "bbb/pwm.h"
#include <mraa.h>
#include "nmea.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
int main()
{
	mraa_uart_context gps;
	load_device_tree("ADAFRUIT-UART1");
	gps = mraa_uart_init_raw("/dev/ttyO1");
    mraa_uart_set_baudrate(gps, 9600);
    char buf[1000];
    char search[7];
    gprmc_t readGPS;
   // char *p = buf;
    while(1){//gprmc_t readGPS;
             int i=0;
            mraa_uart_read(gps, search, 1);
            if(search[0] == '$'){
            	for(i=1; i<7;i++){
            		mraa_uart_read(gps, search+i, 1);	
            	}
            if(strstr(search, "$GPRMC,")){
           		for(i=0; i<100;i++){
           			mraa_uart_read(gps, buf+i, 1);
           			if(buf[i] == '\n'){
           				buf[i]='\0';
           				break;
           			}
            		
            	}
            	// printf("%s\n", buf);
            	nmea_parse_gprmc(buf, &readGPS);
            	gps_convert_deg_to_dec(&(readGPS.latitude), readGPS.lat, &(readGPS.longitude), readGPS.lon);
            	printf("%d\n", readGPS.state);
            	printf("%f\n", readGPS.latitude);
            	printf("%f\n", readGPS.longitude);
    	    	
    	    
    	    }
				
				
			
    	 //    mraa_uart_read(gps, buf+1, 1);
	    	// mraa_uart_read(gps, buf+2, 1);
	    	// mraa_uart_read(gps, buf+3, 1);
	    	// mraa_uart_read(gps, buf+4, 1);
	    	// mraa_uart_read(gps, buf+5, 1);
	    	// mraa_uart_read(gps, buf+6, 1);
	    	// mraa_uart_read(gps, buf+7, 1);
	    	// mraa_uart_read(gps, buf+8, 1);
	    	// mraa_uart_read(gps, buf+9, 1);
	    	// mraa_uart_read(gps, buf+10, 1);
	    	// mraa_uart_read(gps, buf+11, 1);
	    	// mraa_uart_read(gps, buf+12, 1);
	    	// mraa_uart_read(gps, buf+13, 1);
	    	// mraa_uart_read(gps, buf+14, 1);
	    	// mraa_uart_read(gps, buf+15, 1);
	    	// mraa_uart_read(gps, buf+16, 1);
	    // printf("%s\n",buf);	
    	 // if(buf[0]=='$')
    	  // {        
    	  //	printf("%s\n", buf);
    	       //buf = strchr(buf, ',')+1; 
    	        //printf("%s\n", buf);
    // }

    	// if (strchr(buf, '$GPRMC')!=NULL){
    	// 	//buf = strchr(buf, ',')+1; 
    	//         //printf("%s\n", buf);
    	// }
    	       //buf = strchr(buf, ',')+1; 
    	        //printf("%s\n", buf);
     
	    	   //mraa_uart_read(gps, buf, 1);
	    	// mraa_uart_read(gps, buf+1, 1);
	    	// mraa_uart_read(gps, buf+2, 1);
	    	// mraa_uart_read(gps, buf+3, 1);
	    	// mraa_uart_read(gps, buf+4, 1);
	    	// mraa_uart_read(gps, buf+5, 1);
	    	// mraa_uart_read(gps, buf+6, 1);
	    	// mraa_uart_read(gps, buf+7, 1);
	    	// mraa_uart_read(gps, buf+8, 1);
	    	// mraa_uart_read(gps, buf+9, 1);
	    	// mraa_uart_read(gps, buf+10, 1);
	    	// mraa_uart_read(gps, buf+11, 1);
	    	// mraa_uart_read(gps, buf+12, 1);
	    	// mraa_uart_read(gps, buf+13, 1);
	    	// mraa_uart_read(gps, buf+14, 1);
	    	// mraa_uart_read(gps, buf+15, 1);
	    	// mraa_uart_read(gps, buf+16, 1);
	    	// mraa_uart_read(gps, buf+17, 1);
    		//int i=0;
    		//for(i = 0; i<100)
    		// if(nmea_get_message_type(buf)==NMEA_GPRMC){
	    	// nmea_parse_gprmc(buf, &readGPS);
	    	// printf("%d\n", readGPS.speed);}
	    // }
	}
}
}
