#include<stdio.h>                                                                                   
#include<stdlib.h>                                                                                  
#include<unistd.h>                                                                                  
#include<signal.h>                                                                                  
#include<mraa/gpio.h>                                                                               
#include<time.h>                                                                                    
                                                                                                    
sig_atomic_t volatile isrunning =1;                                                                 
void do_when_interrupted(int sig) {
        if (sig == SIGINT)
	            isrunning ==0;
}

long get_distance(mraa_gpio_context trigger, mraa_gpio_context echo)
{
        long distance, time1, time2;
        mraa_gpio_write(trigger, 0);
        usleep(10);
        mraa_gpio_write(trigger, 1);
        usleep(10);
        mraa_gpio_write(trigger, 0);
        while(mraa_gpio_read(echo) == 0){
                     time1 = clock();}
    //    printf("time1 %d\n",time1);}
        while(mraa_gpio_read(echo) == 1){
                     time2 = clock()-time1;}
   //     printf("time2 %d\n",time2);
        if(time2>0&&time2<30000){distance = time2 / 58.82;}
         /* else{                                                             
           get_distance(trigger,echo);                                                             
             } */
	else distance=0;//if get wrong distance
        return distance;
}
int main(){                                                                                         
  long distance_l, distance_c, distance_r;                                                        
  mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r;                                 
   signal(SIGINT, do_when_interrupted);                                                             
                                                                                                    
        trig_l = mraa_gpio_init(10);                                                                 
        echo_l = mraa_gpio_init(11);                                                                 
        trig_c = mraa_gpio_init(7);                                                                
        echo_c = mraa_gpio_init(8);                                                                
        trig_r = mraa_gpio_init(12);                                                                 
        echo_r = mraa_gpio_init(13);                                                                 
        if (trig_c == NULL || echo_c == NULL || trig_l == NULL || echo_l == NULL || trig_r == NULL ||echo_r == NULL){                                                                                          
                fprintf(stderr, "Failed to initialized.\n");                                        
                return 1;}                            
        mraa_gpio_dir(trig_l, MRAA_GPIO_OUT);                                                       
        mraa_gpio_dir(echo_l, MRAA_GPIO_IN);                                                        
        mraa_gpio_dir(trig_c, MRAA_GPIO_OUT);                                                       
        mraa_gpio_dir(echo_c, MRAA_GPIO_IN);                                                        
        mraa_gpio_dir(trig_r, MRAA_GPIO_OUT);                                                       
        mraa_gpio_dir(echo_r, MRAA_GPIO_IN);                                                        
   while(isrunning == 1){                                                                          
                //distance_up_l = get_distance(trig_up_l, echo_up_l);                               
                //distance_up_r = get_distance(trig_up_r, echo_up_r);
		usleep(20);                               
                distance_l = get_distance(trig_l, echo_l);
                usleep(20);		
                distance_c = get_distance(trig_c, echo_c); 
                usleep(20);		
                distance_r = get_distance(trig_r, echo_r);                                          
               printf(" c:%d l:%d r:%d\n", distance_c, distance_l, distance_r);}                                                     
}                                                                                                   
