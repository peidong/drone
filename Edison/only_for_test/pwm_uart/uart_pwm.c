#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mraa.h>
#include <string.h>



int main()
{ 
    
    int pwm1=32980;
    int pwm2=3200;
    int pwm3=2000;
    int pwm4=39900;
    char out_pwm[21];
    char pwm1_c[6];
    char pwm2_c[6];
    char pwm3_c[6];
    char pwm4_c[6];
    
    if(pwm1/10==0)
         sprintf(pwm1_c,"0000%d",pwm1);
    else if(pwm1/100==0)
         sprintf(pwm1_c,"000%d",pwm1);
    else if(pwm1/1000==0)
         sprintf(pwm1_c,"00%d",pwm1);
    else if(pwm1/10000==0)
         sprintf(pwm1_c,"0%d",pwm1);
    else
         sprintf(pwm1_c,"%d",pwm1);
    
    if(pwm2/10==0)
         sprintf(pwm2_c,"%s%d","0000",pwm2);
    else if(pwm2/100==0)
         sprintf(pwm2_c,"%s%d","000",pwm2);
    else if(pwm2/1000==0)
         sprintf(pwm2_c,"%s%d","00",pwm2);
    else if(pwm2/10000==0)
         sprintf(pwm2_c,"%s%d","0",pwm2);
    else
         sprintf(pwm2_c,"%d",pwm2);
    
    if(pwm3/10==0)
         sprintf(pwm3_c,"0000%d",pwm3);
    else if(pwm3/100==0)
         sprintf(pwm3_c,"000%d",pwm3);
    else if(pwm3/1000==0)
         sprintf(pwm3_c,"00%d",pwm3);
    else if(pwm3/10000==0)
         sprintf(pwm3_c,"0%d",pwm3);
    else
         sprintf(pwm3_c,"%d",pwm3);
    
    if(pwm4/10==0)
         sprintf(pwm4_c,"%s%d","0000",pwm4);
    else if(pwm4/100==0)
         sprintf(pwm4_c,"%s%d","000",pwm4);
    else if(pwm4/1000==0)
         sprintf(pwm4_c,"%s%d","00",pwm4);
    else if(pwm4/10000==0)
         sprintf(pwm4_c,"%s%d","0",pwm4);
    else
         sprintf(pwm4_c,"%d",pwm4);
    
    sprintf(out_pwm,"%s%s%s%s",pwm1_c,pwm2_c,pwm3_c,pwm4_c);
    
    printf("%s",out_pwm);
    
    
    //printf("%s",r);
    mraa_init(); // can we put it in the beginning. Avoid repeating definition.
    mraa_uart_context pwm12;
    pwm12 = mraa_uart_init(0);
    mraa_uart_set_baudrate(pwm12,9600);

    while(1){
            mraa_uart_write(pwm12,out_pwm,21);
            usleep(1000000);
            }
    return 0;
}
    