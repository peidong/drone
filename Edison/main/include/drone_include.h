#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash/uthash.h"
#include "http/http.h"
#include <json-c/json.h>
#include "thread/thpool.h" /*https://github.com/Pithikos/C-Thread-Pool*/
#include <unistd.h>/*usleep*/
#include <math.h>//round
#include <time.h>//nanosleep
#define MPU9250_UART
//#define MPU9250_I2C
#ifdef MPU9250_UART
#include "mpu9250/mpu9250_uart.h"  //mpu9250 uart
#endif
#ifdef MPU9250_I2C
#include "mpu9250/mpu9250.h"  //mpu9250 i2c
#endif
#include "pid/pid.h"  //include pid file
#include "timer/timer.h" //timer
#include <mraa.h>
#include <stdint.h>
/**
 * print debug
 */
// #define PRINT_DEBUG_PWM_HTTP_GET
//#define PRINT_DEBUG_YAW_PITCH_ROLL
//#define PRINT_DEBUG_PID_CHANGE
// #define PRINT_DEBUG_PID_TUNING
// #define PRINT_DEBUG_PWM
//#define PRINT_DEBUG_THREAD
// #define PRINT_CAR_MANUAL
//#define TIMER
//#define TIMER_YAW_PITCH_ROLL
//#define TIMER_PID

/**
 * define value
 */
#define PWM_DEVIDE_RATIO 1
#define PWM_MANUAL_CHANGE_AMOUNT 0.000025
#define PWM_MANUAL_CHANGE_AMOUNT_LARGE 0.000025*80
#define PWM_INITIAL 0.000025*4
#define PWM_MIN 0.000025*500
#define PWM_RANGE 0.000025*200

int n_index_yaw_pitch_roll = 0;
#ifdef TIMER
custom_timer_t g_timer;
long g_last_time_us = 0;
#endif

/**
 * struct drone
 */
struct T_drone{
    /**
     * This is for flag
     */
    int nflag_stop_all;
    int nflag_enable_pwm_pid_ultrasound;
    /**
     * These following are from server
     */
    char *sz_mac_address;
    int n_control_type;
    int n_auto_control_command;
    int n_manual_control_command;
    double arrd_suspend_pwm[4];
    /**
     * These following are from the board itself
     */
    double arrd_current_pwm[4];
    double arrd_current_pwm_min[4];
    //double arrd_last_pwm[4];
    double arrd_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
    int16_t n_grawx;
    int16_t n_grawy;
    int16_t n_grawz;
    double arrd_pid_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
    int arrn_ultrasound[6];/*0:up 1:down 2:left 3:right 4:forward 5:backward*/
    /**
     * GPS info
     */
    double d_current_latitude;
    double d_current_longitude;
    double d_face_direction;
    double d_destination_latitude;
    double d_destination_longitude;
    double d_move_direction;
    /**
     * ultrasonic info
     */
    double d_distance_left;
    double d_distance_center;
    double d_distance_right;
    double d_distance_slight_left;
    double d_distance_slight_right;
    int n_ultrasonic_degree;
    /**
     * pid variable
     */
    double d_kp_pitch;
    double d_ki_pitch;
    double d_kd_pitch;
    double d_kp_roll;
    double d_ki_roll;
    double d_kd_roll;
    double d_kp_yaw;
    double d_ki_yaw;
    double d_kd_yaw;
};
/**
 * global struct
 */
struct T_drone g_T_drone_self;
/**
 * @param pointer of struct T_drone
 * @return 0
 * initialize the struct
 */
int initialize_struct_T_drone(struct T_drone *pT_drone){
    /**
     * This is for flag
     */
    pT_drone->nflag_stop_all = 0;
    pT_drone->nflag_enable_pwm_pid_ultrasound = 0;
    /**
     * These following are from server
     */
    pT_drone->sz_mac_address = "fc:c2:de:3d:7f:af";
    pT_drone->n_control_type = -1;
    pT_drone->n_auto_control_command = -1;
    pT_drone->n_manual_control_command = -1;

    pT_drone->arrd_suspend_pwm[0] = 0;
    pT_drone->arrd_suspend_pwm[1] = 0;
    pT_drone->arrd_suspend_pwm[2] = 0;
    pT_drone->arrd_suspend_pwm[3] = 0;

    pT_drone->arrd_current_pwm[0] = PWM_INITIAL;
    pT_drone->arrd_current_pwm[1] = PWM_INITIAL;
    pT_drone->arrd_current_pwm[2] = PWM_INITIAL;
    pT_drone->arrd_current_pwm[3] = PWM_INITIAL;

    pT_drone->arrd_current_pwm_min[0] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[1] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[2] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[3] = PWM_INITIAL;

    pT_drone->arrd_yaw_pitch_roll[0] = 0;
    pT_drone->arrd_yaw_pitch_roll[1] = 0;
    pT_drone->arrd_yaw_pitch_roll[2] = 0;

    pT_drone->n_grawx = 0;
    pT_drone->n_grawy = 0;
    pT_drone->n_grawz = 0;

    pT_drone->arrd_pid_yaw_pitch_roll[0] = 0;
    pT_drone->arrd_pid_yaw_pitch_roll[1] = 0;
    pT_drone->arrd_pid_yaw_pitch_roll[2] = 0;

    pT_drone->arrn_ultrasound[0] = 0;
    pT_drone->arrn_ultrasound[1] = 0;
    pT_drone->arrn_ultrasound[2] = 0;
    pT_drone->arrn_ultrasound[3] = 0;
    pT_drone->arrn_ultrasound[4] = 0;
    pT_drone->arrn_ultrasound[5] = 0;

    pT_drone->d_current_latitude = 0;
    pT_drone->d_current_longitude = 0;
    pT_drone->d_face_direction = 0;
    pT_drone->d_destination_latitude = 0;
    pT_drone->d_destination_longitude = 0;
    pT_drone->d_move_direction = 0;

    pT_drone->d_distance_left = 0;
    pT_drone->d_distance_center = 0;
    pT_drone->d_distance_right = 0;
    pT_drone->d_distance_slight_left = 0;
    pT_drone->d_distance_slight_right = 0;
    pT_drone->n_ultrasonic_degree = 0;

    pT_drone->d_kp_pitch = 0;
    pT_drone->d_ki_pitch = 0;
    pT_drone->d_kd_pitch = 0;
    pT_drone->d_kp_roll = 0;
    pT_drone->d_ki_roll = 0;
    pT_drone->d_kd_roll = 0;
    pT_drone->d_kp_yaw = 0;
    pT_drone->d_ki_yaw = 0;
    pT_drone->d_kd_yaw = 0;
    return 0;
}
/**
 * set all pwm values to 0 when stop flag is true
 */
int initialize_pwm_value(struct T_drone *pT_drone){
    pT_drone->arrd_current_pwm[0] = PWM_INITIAL;
    pT_drone->arrd_current_pwm[1] = PWM_INITIAL;
    pT_drone->arrd_current_pwm[2] = PWM_INITIAL;
    pT_drone->arrd_current_pwm[3] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[0] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[1] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[2] = PWM_INITIAL;
    pT_drone->arrd_current_pwm_min[3] = PWM_INITIAL;
    return 0;
}
/**
 * manually update the pwm value using by test purpose
 */
int update_T_drone_http_pwm_get(struct T_drone *pT_drone){
    char *sz_url_get_pwm = "http://128.97.89.181/rest/api/pwm/get/";
    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_pwm[4], *pT_json_object_data, *pT_json_object_update_time;
    int n_json_response;
    int n_index=0;
    /**
     * start http_get
     */
    sz_http_response = http_get(sz_url_get_pwm);
    /**
     * store the response in json
     */
    pT_json_object_whole_response = json_tokener_parse(sz_http_response);
    /**
     * extract the values in json format
     */
    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm1", &ppT_json_object_pwm[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm2", &ppT_json_object_pwm[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm3", &ppT_json_object_pwm[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm4", &ppT_json_object_pwm[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "update_time", &pT_json_object_update_time);
    /**
     * store the pwm values in the struct array
     */
    for(n_index = 0; n_index < 4; n_index++)
    {
        pT_drone->arrd_current_pwm[n_index] = (json_object_get_double(*(ppT_json_object_pwm+n_index))) / PWM_DEVIDE_RATIO;
    }
    return 0;
}
/**
 * update the pwm value using by test purpose
 */
int update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    char *sz_url_post_pwm = "http://128.97.89.181/rest/api/pwm/post/";
    char arrc_post_data[100];
    /**
     * store the post data
     */
    sprintf(arrc_post_data, "pwm1=%f&pwm2=%f&pwm3=%f&pwm4=%f", pT_drone->arrd_current_pwm[0], pT_drone->arrd_current_pwm[1], pT_drone->arrd_current_pwm[2], pT_drone->arrd_current_pwm[3]);
    /**
     * http post
     */
    http_post(sz_url_post_pwm, arrc_post_data);
    return 0;
}
/**
 * update the pid_tuning value using by test purpose
 */
int update_T_drone_http_pid_tuning_get(struct T_drone *pT_drone){
    char *sz_url_get_pid_tuning = "http://128.97.89.181/rest/api/pid_tuning/get/";

    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_pid_tuning[9], *pT_json_object_data, *pT_json_object_update_time;
    int n_json_response;

    sz_http_response = http_get(sz_url_get_pid_tuning);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kp_pitch", &ppT_json_object_pid_tuning[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "ki_pitch", &ppT_json_object_pid_tuning[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kd_pitch", &ppT_json_object_pid_tuning[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kp_roll", &ppT_json_object_pid_tuning[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "ki_roll", &ppT_json_object_pid_tuning[4]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kd_roll", &ppT_json_object_pid_tuning[5]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kp_yaw", &ppT_json_object_pid_tuning[6]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "ki_yaw", &ppT_json_object_pid_tuning[7]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kd_yaw", &ppT_json_object_pid_tuning[8]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "update_time", &pT_json_object_update_time);

    pT_drone->d_kp_pitch = json_object_get_double(*(ppT_json_object_pid_tuning + 0));
    pT_drone->d_ki_pitch = json_object_get_double(*(ppT_json_object_pid_tuning + 1));
    pT_drone->d_kd_pitch = json_object_get_double(*(ppT_json_object_pid_tuning + 2));
    pT_drone->d_kp_roll = json_object_get_double(*(ppT_json_object_pid_tuning + 3));
    pT_drone->d_ki_roll = json_object_get_double(*(ppT_json_object_pid_tuning + 4));
    pT_drone->d_kd_roll = json_object_get_double(*(ppT_json_object_pid_tuning + 5));
    pT_drone->d_kp_yaw = json_object_get_double(*(ppT_json_object_pid_tuning + 6));
    pT_drone->d_ki_yaw = json_object_get_double(*(ppT_json_object_pid_tuning + 7));
    pT_drone->d_kd_yaw = json_object_get_double(*(ppT_json_object_pid_tuning + 8));
    return 0;
}

/**
 * update the drone value
 */
int update_T_drone_http(struct T_drone *pT_drone){
    //How to concat two char* string in C program
    //http://stackoverflow.com/questions/18468229/how-to-concat-two-char-string-in-c-program
    //sz_mac_address = "fc:c2:de:3d:7f:af";
    char *sz_url_get_control_part1 = "http://128.97.89.181/rest/api/control/get/?mac_address=";
    char *sz_url_post_control_part1 = "http://128.97.89.181/rest/api/control/post/?mac_address=";
    /**
     * get url
     */
    char *sz_url_get_control = NULL;
    if(NULL == sz_url_get_control){
        sz_url_get_control = (char*) malloc(1 + strlen(sz_url_get_control_part1) + strlen(pT_drone->sz_mac_address));
    }
    strcpy(sz_url_get_control, sz_url_get_control_part1);
    strcat(sz_url_get_control, pT_drone->sz_mac_address);
    /**
     * post url
     */
    char *sz_url_post_control = NULL;
    if (NULL == sz_url_post_control){
        sz_url_post_control = (char*) malloc(1 + strlen(sz_url_post_control_part1) + strlen(pT_drone->sz_mac_address));
    }
    // char *sz_url_post_control = (char*) malloc(1 + strlen(sz_url_post_control_part1) + strlen(pT_drone->sz_mac_address));
    strcpy(sz_url_post_control, sz_url_post_control_part1);
    strcat(sz_url_post_control, pT_drone->sz_mac_address);

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
    for(n_index = 0; n_index < 4; n_index++){
        pT_drone->arrd_suspend_pwm[n_index] = json_object_get_double(*(ppT_json_object_suspend_pwm + n_index));
    }

    /**
     * stop
     */
    if (pT_drone->n_manual_control_command == 10){
        pT_drone->nflag_stop_all = 1;
        /**
         * set the manual control command back to server
         */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }

    /**
     * pwm manual change, 11 small up, 12 small down, 13 big up, 14 big down
     */
    if (pT_drone->n_manual_control_command == 11 || pT_drone->n_manual_control_command == 12 || pT_drone->n_manual_control_command == 13 || pT_drone->n_manual_control_command == 14){
        if (pT_drone->n_manual_control_command == 11){
            for(n_index=0; n_index<4; n_index++){
                pT_drone->arrd_current_pwm[n_index] += PWM_MANUAL_CHANGE_AMOUNT;
                pT_drone->arrd_current_pwm_min[n_index] += PWM_MANUAL_CHANGE_AMOUNT;
                //limit the value range
                if(pT_drone->arrd_current_pwm_min[n_index] > PWM_MIN){
                    pT_drone->arrd_current_pwm_min[n_index] = PWM_MIN;
                }
                if(pT_drone->arrd_current_pwm[n_index] > pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE){
                    pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE;
                }
            }
        }else if (pT_drone->n_manual_control_command == 12){
            for(n_index=0; n_index<4; n_index++){
                pT_drone->arrd_current_pwm[n_index] -= PWM_MANUAL_CHANGE_AMOUNT;
                pT_drone->arrd_current_pwm_min[n_index] -= PWM_MANUAL_CHANGE_AMOUNT;
                //limit the value range
                if(pT_drone->arrd_current_pwm_min[n_index] < PWM_INITIAL){
                    pT_drone->arrd_current_pwm_min[n_index] = PWM_INITIAL;
                }
                if(pT_drone->arrd_current_pwm[n_index] < pT_drone->arrd_current_pwm_min[n_index]){
                    pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index];
                }
            }
        }else if (pT_drone->n_manual_control_command == 13){
            for(n_index=0; n_index<4; n_index++){
                pT_drone->arrd_current_pwm[n_index] += PWM_MANUAL_CHANGE_AMOUNT_LARGE;
                pT_drone->arrd_current_pwm_min[n_index] += PWM_MANUAL_CHANGE_AMOUNT_LARGE;
                //limit the value range
                if(pT_drone->arrd_current_pwm_min[n_index] > PWM_MIN){
                    pT_drone->arrd_current_pwm_min[n_index] = PWM_MIN;
                }
                if(pT_drone->arrd_current_pwm[n_index] > pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE){
                    pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE;
                }
            }
        }else if (pT_drone->n_manual_control_command == 14){
            for(n_index=0; n_index<4; n_index++){
                pT_drone->arrd_current_pwm[n_index] -= PWM_MANUAL_CHANGE_AMOUNT_LARGE;
                pT_drone->arrd_current_pwm_min[n_index] -= PWM_MANUAL_CHANGE_AMOUNT_LARGE;
                //limit the value range
                if(pT_drone->arrd_current_pwm_min[n_index] < PWM_INITIAL){
                    pT_drone->arrd_current_pwm_min[n_index] = PWM_INITIAL;
                }
                if(pT_drone->arrd_current_pwm[n_index] < pT_drone->arrd_current_pwm_min[n_index]){
                    pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index];
                }
            }
        }
        /**
         * set the manual control command back to server
         */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }

    /**
     * nflag_enable_pwm_pid_ultrasound
     */
    if(pT_drone->n_manual_control_command == 16 || pT_drone->n_manual_control_command == 17){
        if(pT_drone->n_manual_control_command == 16){
            pT_drone->nflag_enable_pwm_pid_ultrasound = 1;
        }else if(pT_drone->n_manual_control_command == 17){
            pT_drone->nflag_enable_pwm_pid_ultrasound = 0;
        }
        /**
         * set the manual control command back to server
         */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }
    /**
     * free pointer
     */
    if(sz_url_get_control != NULL){
        free(sz_url_get_control);
        sz_url_get_control = NULL;
    }
    if(sz_url_post_control != NULL){
        free(sz_url_post_control);
        sz_url_post_control = NULL;
    }
    return 0;
}

/**
 *
 * update the drone value for gps
 *
 */
int update_T_drone_http_gps(struct T_drone *pT_drone){
    //How to concat two char* string in C program
    //http://stackoverflow.com/questions/18468229/how-to-concat-two-char-string-in-c-program
    char *sz_url_get_gps_destination = "http://128.97.89.181/rest/api/gps/get/?location_type=0";//get destination gps
    char *sz_url_get_gps_iPhone = "http://128.97.89.181/rest/api/gps/get/?location_type=1";//get iPhone(Edison board) current gps
    //char *sz_url_post_gps = "http://128.97.89.181/rest/api/gps/post/?location_type=1";

    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *pT_json_object_data, *pT_json_object_update_time, *pT_json_object_face_direction, *pT_json_object_latitude, *pT_json_object_longitude/* ,*pT_json_object_stop_sign */;
    int n_json_response;

    sz_http_response = http_get(sz_url_get_gps_iPhone);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "face_direction", &pT_json_object_face_direction);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "latitude", &pT_json_object_latitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "longitude", &pT_json_object_longitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "update_time",&pT_json_object_update_time);

    pT_drone->d_face_direction = json_object_get_double(pT_json_object_face_direction);
    pT_drone->d_current_latitude = json_object_get_double(pT_json_object_latitude);
    pT_drone->d_current_longitude = json_object_get_double(pT_json_object_longitude);

    sz_http_response = http_get(sz_url_get_gps_destination);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "face_direction", &pT_json_object_face_direction);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "latitude", &pT_json_object_latitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "longitude", &pT_json_object_longitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "update_time",&pT_json_object_update_time);

    pT_drone->d_destination_latitude = json_object_get_double(pT_json_object_latitude);
    pT_drone->d_destination_longitude = json_object_get_double(pT_json_object_longitude);

    return 0;
}



/**
 * 0:up 1:down 2:left 3:right 4:forward 5:backward
 */
int update_T_drone_arrn_ultrasound(struct T_drone *pT_drone){
    return 0;
}
#ifdef MPU9250_UART
int update_T_drone_arrd_yaw_pitch_roll_uart(struct T_drone *pT_drone){
#ifdef TIMER_YAW_PITCH_ROLL
    timer_start(&g_timer);
#endif
    int16_t arawx,arawy,arawz;
    int16_t grawx,grawy,grawz;
    int16_t mrawx,mrawy,mrawz;
    float ax,ay,az,gx,gy,gz,mx,my,mz;
    float yaw, pitch, roll;
    float result[2001][4];
    int sample = 0;
    int n_not_find_header_times = 0;
    int n_find_header_times = 0;
    int n_not_available_times = 0;

    mraa_uart_context uno;
    uno = mraa_uart_init(0);

    mraa_uart_set_baudrate(uno, 115200);    // Really have no idea why higher baud does not work! And if 230400, the whole terminal crashes!

    char read[36];
    // char daer[18];
    char flag[1];
    usleep(1000);
    while(1)
    {
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_yaw pitch roll\n");
#endif
        // printf("%s\n",mraa_get_version());
#ifdef TIMER_YAW_PITCH_ROLL
        g_last_time_us = timer_delta_us(&g_timer);
        // timer_unpause(&g_timer);
#endif
    if(mraa_uart_data_available(uno, 0))
    {
        n_not_available_times = 0;
        if(n_find_header_times>=55)
        {
            // usleep(5000);
            // n_find_header_times=0;
        }

        mraa_uart_read(uno,flag,1);

        if(flag[0]==' '){   // ' ' is the beginning of the data package. Once detecting the header, reading begins!!!
            n_not_find_header_times = 0;
            n_find_header_times++;
if(mraa_uart_data_available(uno, 0) != 1){
    //printf("not not not hahaha\n");
}
            mraa_uart_read(uno,read,36);


            arawx = -(myatoi(read[0])<<4|myatoi(read[1]))<<8|(myatoi(read[2])<<4|myatoi(read[3]));
            arawy = -(myatoi(read[4])<<4|myatoi(read[5]))<<8|(myatoi(read[6])<<4|myatoi(read[7]));
            arawz = (myatoi(read[8])<<4|myatoi(read[9]))<<8|(myatoi(read[10])<<4|myatoi(read[11]));

            grawx = (myatoi(read[12])<<4|myatoi(read[13]))<<8|(myatoi(read[14])<<4|myatoi(read[15]));
            grawy = (myatoi(read[16])<<4|myatoi(read[17]))<<8|(myatoi(read[18])<<4|myatoi(read[19]));
            grawz = (myatoi(read[20])<<4|myatoi(read[21]))<<8|(myatoi(read[22])<<4|myatoi(read[23]));

            pT_drone->n_grawx = grawx;
            pT_drone->n_grawy = grawy;
            pT_drone->n_grawz = grawz;

            mrawx = (myatoi(read[26])<<4|myatoi(read[27]))<<8|(myatoi(read[24])<<4|myatoi(read[25]));
            mrawy = (myatoi(read[30])<<4|myatoi(read[31]))<<8|(myatoi(read[28])<<4|myatoi(read[29]));
            mrawz = (myatoi(read[34])<<4|myatoi(read[35]))<<8|(myatoi(read[32])<<4|myatoi(read[33]));

            // printf("%d\t,%d\t,%d\t,%d\t,%d\t,%d\t,%d\t,%d\t,%d\t\n",arawx,arawy,arawz,grawx,grawy,grawz,mrawx,mrawy,mrawz);

            ax = (float)arawx*aRes;
            ay = (float)arawy*aRes;
            az = (float)arawz*aRes;
            gx = (float)grawx*gRes;
            gy = (float)grawy*gRes;
            gz = (float)grawz*gRes;
            mx = (float)mrawx*mRes*magxCalibration - 406 - 49 - 150 + 72 - 13;  // get actual magnetometer value, this depends on scale being set
            my = (float)mrawy*mRes*magyCalibration - 95 + 43 + 15 - 178 + 87;
            mz = (float)mrawz*mRes*magzCalibration + 370 - 72 + 403 - 447 + 207;

            // printf("%.1f\t%.1f\t%.1f\n", mx, my, mz);
            /*
                          if(sample < 2000)
                          {
                          result[sample][0] = (float)(arawx)/1000;
                          result[sample][1] = (float)(arawy)/1000;
                          result[sample][2] = (float)(arawz)/1000;
            // sample++;
            }
            */

            // AHRS
            MadgwickAHRSupdate(ax, ay, az, gx*PI / 180.0f, gy*PI / 180.0f, gz*PI / 180.0f, my, mx, mz); //my, mx, mz
            // Calculate yaw pitch roll
            yaw = atan2(2.0f * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);
            pitch = -asin(2.0f * (q1 * q3 - q0 * q2));
            roll = atan2(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);
            yaw *= 180.0f / PI;
            pitch *= 180.0f / PI;
            roll *= 180.0f / PI;
            if (yaw<0) yaw += 360;
            /*
               if(sample < 2000){
               result[sample][3] = roll;
            // result[sample][1] = ay;
            // result[sample][2] = az;
            sample++;
            }

            if(sample == 2000){
            printf("Outputing data of mag!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            FILE* fp;
            int i,j;
            fp = fopen("demo.txt", "w");
            for (i = 0; i < 2000; i++){
            for (j = 0; j < 4; j++){
            fprintf(fp, "%.5f ", result[i][j]);
            }
            fputc('\n', fp);
            }
            fclose(fp);
            printf("Finish!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            sample = 2001;
            }
            */
            //usleep(1000);

            pT_drone->arrd_yaw_pitch_roll[0] = yaw;
            pT_drone->arrd_yaw_pitch_roll[1] = pitch;
            pT_drone->arrd_yaw_pitch_roll[2] = roll;

            // usleep(3000);

#ifdef PRINT_DEBUG_YAW_PITCH_ROLL
            //if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            //n_index_yaw_pitch_roll++;
            //n_index_yaw_pitch_roll = n_index_yaw_pitch_roll%10;
            //if(n_index_yaw_pitch_roll == 0){
            printf("yaw = %.1f\tpitch = %.1f\troll = %.1f\n",yaw, pitch, roll);
            //}
            //}
#endif
#ifdef TIMER_YAW_PITCH_ROLL
        // timer_pause(&g_timer);
        printf("correct times: %d\t Delta (us): %ld\n",n_find_header_times, (timer_delta_us(&g_timer) - g_last_time_us));
#endif
        }
        else{
            // usleep(2000);

            n_find_header_times = 0;
            n_not_find_header_times++;
#ifdef TIMER_YAW_PITCH_ROLL
        // timer_pause(&g_timer);
        printf("wrong times: %d\t Delta (us): %ld\n",n_not_find_header_times, (timer_delta_us(&g_timer) - g_last_time_us));
#endif
        }
    }else{
        n_not_available_times++;
#ifdef TIMER_YAW_PITCH_ROLL
        // timer_pause(&g_timer);
        printf("not available times: %d\t Delta (us): %ld\n",n_not_available_times, (timer_delta_us(&g_timer) - g_last_time_us));
#endif
    }

    }
    mraa_uart_stop(uno);
    return 0;
}
#endif

#ifdef MPU9250_I2C
int update_T_drone_arrd_yaw_pitch_roll_i2c(struct T_drone *pT_drone){
    // mraa_gpio_context gpio_vcc;
    // gpio_vcc = mraa_gpio_init(2);
    // mraa_gpio_mode(gpio_vcc, MRAA_GPIO_PULLDOWN);
    // mraa_gpio_dir(gpio_vcc, MRAA_GPIO_OUT);
    // mraa_gpio_write(gpio_vcc, 1);
    // usleep(100000);
    // mraa_gpio_write(gpio_vcc, 0);
    // usleep(1000000);
    MPU_init();
    while (1)
    {
        if (pT_drone->nflag_stop_all != 0)
        {
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_yaw pitch roll\n");
#endif
        uint8_t Buf[14];
        mraa_i2c_read_bytes_data(mpu, 59, Buf, 14);
        // Accelerometer
        int16_t arawx = -(Buf[0] << 8 | Buf[1]) - 170 + 250;
        int16_t arawy = -(Buf[2] << 8 | Buf[3]) + 600 - 300;
        int16_t arawz = Buf[4] << 8 | Buf[5];
        // Gyroscope
        int16_t grawx = (Buf[8] << 8 | Buf[9]) - 25;
        int16_t grawy = (Buf[10] << 8 | Buf[11]) - 2;
        int16_t grawz = (Buf[12] << 8 | Buf[13]) + 9;

        pT_drone->n_grawx = grawx;
        pT_drone->n_grawy = grawy;
        pT_drone->n_grawz = grawz;

        // Magnetometer
        mraa_i2c_read_bytes_data(mpu, 73, Buf, 6);
        int16_t mrawx = (Buf[1] << 8 | Buf[0]);//-213;// + mag_offset_x;
        int16_t mrawy = (Buf[3] << 8 | Buf[2]);//-92;// + mag_offset_y;
        int16_t mrawz = (Buf[5] << 8 | Buf[4]);//+200;// + mag_offset_z;
        //int result_agm[9] = { arawx, arawy, arawz, grawx, grawy, grawz, mrawx, mrawy, mrawz };

        //printf("%6d,%6d,%6d\n",arawx, arawy, arawz);
        //printf("%6d,%6d,%6d\n",grawx, grawy, grawz);
        float ax = (float)arawx*aRes;
        float ay = (float)arawy*aRes;
        float az = (float)arawz*aRes;
        float gx = (float)grawx*gRes;
        float gy = (float)grawy*gRes;
        float gz = (float)grawz*gRes;
        float mx = (float)mrawx*mRes*magCalibration[0] - 406 - 49 - 150;  // get actual magnetometer value, this depends on scale being set
        float my = (float)mrawy*mRes*magCalibration[1] - 95 + 43 + 15;
        float mz = (float)mrawz*mRes*magCalibration[2] + 370 - 72 + 403;
        //printf("%.1f,%.1f,%.1f\n",mx,my,mz);
        //    MadgwickQuaternionUpdate(ax,ay,az,gx*PI/180.0f,gy*PI/180.0f,gz*PI/180.0f,my,mx,mz);
        MadgwickAHRSupdate(ax, ay, az, gx*PI / 180.0f, gy*PI / 180.0f, gz*PI / 180.0f, my, mx, mz); //my, mx, mz
        q[0] = q0; q[1] = q1; q[2] = q2; q[3] = q3;
        float yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
        float pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
        float roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
        yaw *= 180.0f / PI;
        pitch *= 180.0f / PI;
        roll *= 180.0f / PI;

        if (yaw<0) yaw += 360;

        //    yaw   -= 13.8; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
        //    pitch -= 0.5;
        //    roll -= 1.9;

        pT_drone->arrd_yaw_pitch_roll[0] = yaw;
        pT_drone->arrd_yaw_pitch_roll[1] = pitch;
        pT_drone->arrd_yaw_pitch_roll[2] = roll;
#ifdef PRINT_DEBUG_YAW_PITCH_ROLL
        //if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
        printf("yaw = %.1f\tpitch = %.1f\troll = %.1f\n",yaw, pitch, roll);
        //}
#endif
    }
    mraa_i2c_stop(mpu);
    // mraa_gpio_close(gpio_vcc);
    return 0;
}
#endif
//void test(struct T_drone *pT_drone){
    //timer_start(&g_timer);
    //timer_pause(&g_timer);
    //g_last_time_us = timer_delta_us(&g_timer);
    //long long_duration_time;
    //int n_while_loop_index = 0;
    //while(1){
        //timer_pause(&g_timer);
        //n_while_loop_index++;
        ////printf("Delta (us): %ld\n", timer_delta_us(&g_timer) - g_last_time_us);
        //long_duration_time = timer_delta_us(&g_timer) - g_last_time_us;
        //if(long_duration_time >= 100000){
            //printf("while loops index is %d,\tlong_duration_time (us) is %ld\n",n_while_loop_index, long_duration_time);
            //g_last_time_us = timer_delta_us(&g_timer);
            //n_while_loop_index = 0;
        //}
        //timer_unpause(&g_timer);
    //}
//}

int update_T_drone_arrd_pid(struct T_drone *pT_drone){
//#ifdef TIMER_PID
    //timer_start(&g_timer);
//#endif
    //pidData_t *pidData_yaw = NULL, *pidData_pitch = NULL, *pidData_roll = NULL;
    //if(NULL == pidData_yaw){
        //pidData_yaw = (pidData_t*) malloc(sizeof(pidData_t));
    //}
    //if(NULL == pidData_pitch){
        //pidData_pitch = (pidData_t*) malloc(sizeof(pidData_t));
    //}
    //if(NULL == pidData_roll){
        //pidData_roll = (pidData_t*) malloc(sizeof(pidData_t));
    //}

    //double kp_pitch, ki_pitch, kd_pitch, kp_roll, ki_roll, kd_roll, kp_yaw, ki_yaw, kd_yaw;
    //ctrlDir_t controllerDir;
    //uint32_t samplePeriodMs;
    //int n_index;

    // For the nine values, if we can modify them in IOS app, tests can be easier!
    //kp_pitch = pT_drone->d_kp_pitch;
    //ki_pitch = pT_drone->d_ki_pitch;
    //kd_pitch = pT_drone->d_kd_pitch;
    //kp_roll = pT_drone->d_kp_roll;
    //ki_roll = pT_drone->d_ki_roll;
    //kd_roll = pT_drone->d_kd_roll;
    //kp_yaw  = pT_drone->d_kp_yaw;
    //ki_yaw  = pT_drone->d_ki_yaw;
    //kd_yaw  = pT_drone->d_kd_yaw;

    //samplePeriodMs = 20; //need to be setup
    //// samplePeriodMs = 100; //need to be setup
    //controllerDir = PID_DIRECT; //Direct control not reverse.

    //Pid_Init(pidData_yaw, kp_yaw, ki_yaw, kd_yaw, controllerDir, samplePeriodMs);

    //Pid_Init(pidData_pitch, kp_pitch, ki_pitch, kd_pitch, controllerDir, samplePeriodMs);

    //Pid_Init(pidData_roll, kp_roll, ki_roll, kd_roll, controllerDir, samplePeriodMs);

    while(1){
//#ifdef TIMER_PID
        //g_last_time_us = timer_delta_us(&g_timer);
        //timer_unpause(&g_timer);
//#endif
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            //continue;
        //}
//#ifdef PRINT_DEBUG_THREAD
        //printf("Pid Thread\n");
//#endif
        //kp_pitch = pT_drone->d_kp_pitch;
        //ki_pitch = pT_drone->d_ki_pitch;
        //kd_pitch = pT_drone->d_kd_pitch;
        //kp_roll = pT_drone->d_kp_roll;
        //ki_roll = pT_drone->d_ki_roll;
        //kd_roll = pT_drone->d_kd_roll;
        //kp_yaw  = pT_drone->d_kp_yaw;
        //ki_yaw  = pT_drone->d_ki_yaw;
        //kd_yaw  = pT_drone->d_kd_yaw;
//#ifdef PRINT_DEBUG_PID_TUNING
        //printf("p_pitch = %f\t", pT_drone->d_kp_pitch);
        //printf("i_pitch = %f\t", pT_drone->d_ki_pitch);
        //printf("d_pitch = %f\t", pT_drone->d_kd_pitch);
        //printf("p_roll = %f\t", pT_drone->d_kp_roll);
        //printf("i_roll = %f\t", pT_drone->d_ki_roll);
        //printf("d_roll = %f\t", pT_drone->d_kd_roll);
        //printf("p_yaw = %f\t", pT_drone->d_kp_yaw);
        //printf("i_yaw = %f\t", pT_drone->d_ki_yaw);
        //printf("d_yaw = %f\n", pT_drone->d_kd_yaw);
//#endif
        //Pid_SetTunings(pidData_yaw, kp_yaw, ki_yaw, kd_yaw);
        //Pid_SetTunings(pidData_pitch, kp_pitch, ki_pitch, kd_pitch);
        //Pid_SetTunings(pidData_roll, kp_roll, ki_roll, kd_roll);

        ////"0" is the setpoint or the destination of the final attitude, representing hovering or suspending.
        ////Replace "0" by HTTP request parameters later.

        //// It can be tested after tests for pitch and roll are finished.
        //Pid_SetSetPoint(pidData_yaw, 0);
        //Pid_Run(pidData_yaw, (int)pT_drone->arrd_yaw_pitch_roll[0]);
        //pT_drone->arrd_pid_yaw_pitch_roll[0] = pidData_yaw->output;

        //// For pitch, mainly we can use wires to lock the Y direction. First divide by 2. Adding to pwm1 and pwm2, substracting to pwm3 and pwm4.
        //Pid_SetSetPoint(pidData_pitch, 0);
        //Pid_Run(pidData_pitch, (int)pT_drone->arrd_yaw_pitch_roll[1]);
        //pT_drone->arrd_pid_yaw_pitch_roll[1] = pidData_pitch->output;
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            //continue;
        //}else{
            //pT_drone->arrd_current_pwm[0] += (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
            //pT_drone->arrd_current_pwm[1] += (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);

            //pT_drone->arrd_current_pwm[2] -= (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
            //pT_drone->arrd_current_pwm[3] -= (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
        //}

        //// For roll, mainly we can use wires to lock the X direction. First divide by 2. Adding to pwm1 and pwm3, substracting to pwm2 and pwm4.
        //Pid_SetSetPoint(pidData_roll, 0);
        //Pid_Run(pidData_roll, (int)pT_drone->arrd_yaw_pitch_roll[2]);
        //pT_drone->arrd_pid_yaw_pitch_roll[2] = pidData_roll->output;
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            //continue;
        //}else{
            //pT_drone->arrd_current_pwm[0] -= (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
            //pT_drone->arrd_current_pwm[3] -= (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);

            //pT_drone->arrd_current_pwm[1] += (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
            //pT_drone->arrd_current_pwm[2] += (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
        //}
        /**
         * change pwm to PWM_DEFAULT_VALUE if below 0
         */
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            //continue;
        //}
        //int n_index;
        //for(n_index = 0; n_index < 4; n_index++){
            //if(pT_drone->arrd_current_pwm[n_index] > (pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE)){
                //pT_drone->arrd_current_pwm[n_index] = (pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE);
            //}
            //if(pT_drone->arrd_current_pwm[n_index] < pT_drone->arrd_current_pwm_min[n_index]){
                //pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index];
            //}
        //}
//#ifdef  PRINT_DEBUG_PID_CHANGE
        //printf("pitch change= %f\troll change= %f\n",(pT_drone->arrd_pid_yaw_pitch_roll[1] / 2), (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2));
//#endif
        //usleep(100000); // We need to add some delay to slow down the pid loop. Mainly, 100ms cycle should be good.
//#ifdef TIMER_PID
        //timer_pause(&g_timer);
        //printf("Delta (us): %ld\n", timer_delta_us(&g_timer) - g_last_time_us);
//#endif
    }
    /**
     * free pointer
     */
    //if(pidData_yaw != NULL){
        //free(pidData_yaw);
        //pidData_yaw = NULL;
    //}
    //if(pidData_pitch != NULL){
        //free(pidData_pitch);
        //pidData_pitch = NULL;
    //}
    //if(pidData_roll != NULL){
        //free(pidData_roll);
        //pidData_roll = NULL;
    //}
    return 0;
}


/**
 * Generate pwm wave via uart
 */
int GeneratePwm(struct T_drone *pT_drone){
    /**
     * Initial variables
     */
    int arrn_current_duty[4];
    char arrch_uart_output[21];
    char pwm1_c[6];
    char pwm2_c[6];
    char pwm3_c[6];
    char pwm4_c[6];
    mraa_uart_context uno;
    /**
     * set uno port
     */
    uno = mraa_uart_init(0);
    /**
     * set baudrate, must be 115200
     */
    mraa_uart_set_baudrate(uno,115200);
    while (1){
        /**
         * Thread debug
         */
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_GeneratePwm\n");
#endif
        /**
         * pwm wave duty cycle debug
         */
#ifdef PRINT_DEBUG_PWM
        printf("pwm1 = %f\t", pT_drone->arrd_current_pwm[0]);
        printf("pwm2 = %f\t", pT_drone->arrd_current_pwm[1]);
        printf("pwm3 = %f\t", pT_drone->arrd_current_pwm[2]);
        printf("pwm4 = %f\n", pT_drone->arrd_current_pwm[3]);
#endif
        /**
         * stop all flag
         */
        if (pT_drone->nflag_stop_all != 0){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            memset(arrch_uart_output,'0', 20);
            mraa_uart_write(uno, arrch_uart_output, 21); // write 21 characters uart data
            break;
        }
        else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            memset(arrch_uart_output,'0', 20);
            mraa_uart_write(uno, arrch_uart_output, 21); // write 21 characters uart data
            continue;
        }
        usleep(2000);//2ms motor modulation frequency max = 490Hz
        /**
         * set pwm wave
         */
        arrn_current_duty[0] = (int)(pT_drone->arrd_current_pwm[0] * 40000);
        arrn_current_duty[1] = (int)(pT_drone->arrd_current_pwm[1] * 40000);
        arrn_current_duty[2] = (int)(pT_drone->arrd_current_pwm[2] * 40000);
        arrn_current_duty[3] = (int)(pT_drone->arrd_current_pwm[3] * 40000);

        /**
         * combine four pwm values into one string
         */
        if(arrn_current_duty[0]/10==0)
            sprintf(pwm1_c,"0000%d",arrn_current_duty[0]);
        else if(arrn_current_duty[0]/100==0)
            sprintf(pwm1_c,"000%d",arrn_current_duty[0]);
        else if(arrn_current_duty[0]/1000==0)
            sprintf(pwm1_c,"00%d",arrn_current_duty[0]);
        else if(arrn_current_duty[0]/10000==0)
            sprintf(pwm1_c,"0%d",arrn_current_duty[0]);
        else if(arrn_current_duty[0]/100000==0)
            sprintf(pwm1_c,"%d",arrn_current_duty[0]);

        if(arrn_current_duty[1]/10==0)
            sprintf(pwm2_c,"0000%d",arrn_current_duty[1]);
        else if(arrn_current_duty[1]/100==0)
            sprintf(pwm2_c,"000%d",arrn_current_duty[1]);
        else if(arrn_current_duty[1]/1000==0)
            sprintf(pwm2_c,"00%d",arrn_current_duty[1]);
        else if(arrn_current_duty[1]/10000==0)
            sprintf(pwm2_c,"0%d",arrn_current_duty[1]);
        else if(arrn_current_duty[1]/100000==0)
            sprintf(pwm2_c,"%d",arrn_current_duty[1]);

        if(arrn_current_duty[2]/10==0)
            sprintf(pwm3_c,"0000%d",arrn_current_duty[2]);
        else if(arrn_current_duty[2]/100==0)
            sprintf(pwm3_c,"000%d",arrn_current_duty[2]);
        else if(arrn_current_duty[2]/1000==0)
            sprintf(pwm3_c,"00%d",arrn_current_duty[2]);
        else if(arrn_current_duty[2]/10000==0)
            sprintf(pwm3_c,"0%d",arrn_current_duty[2]);
        else if(arrn_current_duty[2]/100000==0)
            sprintf(pwm3_c,"%d",arrn_current_duty[2]);

        if(arrn_current_duty[3]/10==0)
            sprintf(pwm4_c,"0000%d",arrn_current_duty[3]);
        else if(arrn_current_duty[3]/100==0)
            sprintf(pwm4_c,"000%d",arrn_current_duty[3]);
        else if(arrn_current_duty[3]/1000==0)
            sprintf(pwm4_c,"00%d",arrn_current_duty[3]);
        else if(arrn_current_duty[3]/10000==0)
            sprintf(pwm4_c,"0%d",arrn_current_duty[3]);
        else if(arrn_current_duty[3]/100000==0)
            sprintf(pwm4_c,"%d",arrn_current_duty[3]);

        sprintf(arrch_uart_output,"%s%s%s%s",pwm1_c,pwm2_c,pwm3_c,pwm4_c);

        /**
         * write pwm into serial bus
         */
        if (pT_drone->nflag_stop_all != 0){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            memset(arrch_uart_output,'0', 20);
            mraa_uart_write(uno, arrch_uart_output, 21); // write 21 characters uart data
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            memset(arrch_uart_output,'0', 20);
            mraa_uart_write(uno, arrch_uart_output, 21); // write 21 characters uart data
            continue;
        }else{
            mraa_uart_write(uno, arrch_uart_output, 21);
        }

    }
    /**
     * Reset PWM to 0
     */
    initialize_pwm_value(pT_drone);
    memset(arrch_uart_output,'0', 20);
    mraa_uart_write(uno, arrch_uart_output, 21); // write 21 characters uart data
    mraa_uart_stop(uno);
    return 0;
}

/**
 * Thread Tasks
 */

void ThreadTask_update_T_drone_http_pwm_get(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http_pwm_get\n");
#endif
        update_T_drone_http_pwm_get(pT_drone);
#ifdef PRINT_DEBUG_PWM_HTTP_GET
        printf("pwm1 = %f\t", pT_drone->arrd_current_pwm[0]);
        printf("pwm2 = %f\t", pT_drone->arrd_current_pwm[1]);
        printf("pwm3 = %f\t", pT_drone->arrd_current_pwm[2]);
        printf("pwm4 = %f\n", pT_drone->arrd_current_pwm[3]);
#endif
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http_pwm_post\n");
#endif
        update_T_drone_http_pwm_post(pT_drone);
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_http(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http\n");
#endif
        update_T_drone_http(pT_drone);
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_arrd_pid(struct T_drone *pT_drone){
    update_T_drone_arrd_pid(pT_drone);
}

void ThreadTask_update_T_drone_arrn_ultrasound(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 0){
            continue;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_arrn_ultrasound\n");
#endif
        update_T_drone_arrn_ultrasound(pT_drone);
    }
}

void ThreadTask_update_T_drone_arrd_yaw_pitch_roll(struct T_drone *pT_drone){
#ifdef MPU9250_UART
    update_T_drone_arrd_yaw_pitch_roll_uart(pT_drone);
#endif
#ifdef MPU9250_I2C
    update_T_drone_arrd_yaw_pitch_roll_i2c(pT_drone);
#endif
}

void ThreadTask_GeneratePwm(struct T_drone *pT_drone){
    GeneratePwm(pT_drone);
}

void ThreadTask_update_T_drone_http_gps(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http_gps\n");
#endif
        update_T_drone_http_gps(pT_drone);
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_http_pid_tuning_get(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http_pid_tuning_get\n");
#endif
        update_T_drone_http_pid_tuning_get(pT_drone);
        usleep(50000);
    }
}
