#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.048f
#define RIGHTMAX 0.088f


float speed, turn;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;

void speed_control(mraa_pwm_context in1, mraa_pwm_context in2, float speed) {
	speed = speed / 100;                                  
        if (speed >= 0) {                                   
            mraa_pwm_write(in2, 1.0f);        
            mraa_pwm_write(in1, 1.0f - speed);
        }                                                   
        else if (speed < 0) {                               
            mraa_pwm_write(in1, 1.0f);        
            mraa_pwm_write(in2, 1.0f + speed);
        }    
}

void turn_left(){
    turn = CENTER - LEFT;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}

void turn_right(){
    turn = CENTER - RIGHT;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}

void move_forward(){
    turn = CENTER;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}

void move_backward(){
    turn = CENTER;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}
