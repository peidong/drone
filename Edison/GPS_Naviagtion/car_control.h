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
#include "http/http.h"


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


int update_T_drone_http(struct T_drone *pT_drone){
    //How to concat two char* string in C program
    //http://stackoverflow.com/questions/18468229/how-to-concat-two-char-string-in-c-program
    //sz_mac_address = "fc:c2:de:3d:7f:af";
    char *sz_url_get_control_part1 = "http://fryer.ee.ucla.edu/rest/api/control/get/?mac_address=";
    char *sz_url_get_control = (char*) malloc(1 + strlen(sz_url_get_control_part1) + strlen(pT_drone->sz_mac_address));
    strcpy(sz_url_get_control, sz_url_get_control_part1);
    strcat(sz_url_get_control, pT_drone->sz_mac_address);
    /*char *sz_url_post_control = "http://fryer.ee.ucla.edu/rest/api/control/post/";*/
    
    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_suspend_pwm[4], *pT_json_object_data, *pT_json_object_update_time, *pT_json_object_control_type, *pT_json_object_auto_control_command, *pT_json_object_manual_control_command;
    int n_json_response;
    int n_index=0;

    sz_http_response = http_get(sz_url_get_control);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "control_type", &pT_json_object_control_type);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "auto_control_command", &pT_json_object_auto_control_command);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "manual_control_command", &pT_json_object_manual_control_command);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "suspend_pwm1", &ppT_json_object_suspend_pwm[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "suspend_pwm2", &ppT_json_object_suspend_pwm[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "suspend_pwm3", &ppT_json_object_suspend_pwm[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "suspend_pwm4", &ppT_json_object_suspend_pwm[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"update_time",&pT_json_object_update_time);

    pT_drone->n_control_type = json_object_get_int(pT_json_object_control_type);
    pT_drone->n_auto_control_command = json_object_get_int(pT_json_object_auto_control_command);
    pT_drone->n_manual_control_command = json_object_get_int(pT_json_object_manual_control_command);
    for(n_index = 0; n_index < 4; n_index++)
    {
        pT_drone->arrd_suspend_pwm[n_index] = json_object_get_double(*(ppT_json_object_suspend_pwm + n_index));
    }
    return 0;
}

