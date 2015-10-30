#include "car_control.h"

int main(){
    speed_pwm_in1 = mraa_pwm_init(3);
    speed_pwm_in2 = mraa_pwm_init(5);
    turn_pwm = mraa_pwm_init(6);
	char turn_user_input[MAXBUFSIZ];
    speed = 40;
    
    mraa_pwm_period_us(speed_pwm_in1,870); //1150Hz
	mraa_pwm_enable(speed_pwm_in1, 1);
	mraa_pwm_period_us(speed_pwm_in2,870);
	mraa_pwm_enable(speed_pwm_in2, 1);
	
	mraa_pwm_period_ms(turn_pwm,20);
   	mraa_pwm_enable(turn_pwm, 1);

	mraa_pwm_write(turn_pwm, CENTER);
	mraa_pwm_write(speed_pwm_in1, 1.0f);
	mraa_pwm_write(speed_pwm_in2, 1.0f);


    while(1){
	        printf("Turn (L, C, R): ");                                      
        	scanf("%s", turn_user_input);               
                
    		if (!strcmp(turn_user_input, "L") || !strcmp(turn_user_input, "l"))     
        	    turn_left();
            else if (!strcmp(turn_user_input, "C") || !strcmp(turn_user_input, "c"))
            	move_forward();               
        	else if (!strcmp(turn_user_input, "R") || !strcmp(turn_user_input, "r"))
		        turn_right();
            else {                                 
            		printf("Wrong turn type!\n");
 			return 1;
		}
		
		sleep(1);
		speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
	}
	return 0;

}
