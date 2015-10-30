#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <mraa/pwm.h>
#include <mraa/gpio.h>
#include <sys/time.h>
#include <math.h>
//#include "uthash/uthash.h"
//#include "http/http.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.048f
#define RIGHTMAX 0.088f


    float speed, turn;
    mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;

struct T_drone{
    //These following are from server
    char *sz_mac_address;
    int n_control_type;
    int n_auto_control_command;
    int n_manual_control_command;
    double arrd_suspend_pwm[4];

    //These following are from the board itself
    double arrd_current_pwm[4];
    double arrd_last_pwm[4];
    double arrd_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
    double arrd_pid_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
    int arrn_ultrasound[6];/*0:up 1:down 2:left 3:right 4:forward 5:backward*/
    struct timespec arrT_timespec_high[4];
    struct timespec arrT_timespec_low[4];

    double d_latitude;
    double d_longitude;
    double d_face_direction;
};



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



