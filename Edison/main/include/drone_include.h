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
#include "timer/timer.h" //timer
#include <mraa.h>
#include <stdint.h>

/**
 * print debug
 */
//#define PRINT_DEBUG_PWM_HTTP_GET
//#define PRINT_DEBUG_THREAD
//#define TIMER

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
    int nflag_enable_uart_send;
    int nflag_enable_uart_receive;
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
    /**
     * second loop pid values
     */
    double d_kp_second_pitch;
    double d_kd_second_pitch;
    double d_kp_second_roll;
    double d_kd_second_roll;
    double d_kp_second_yaw;
    double d_kd_second_yaw;
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
    pT_drone->nflag_enable_uart_send = 0;
    pT_drone->nflag_enable_uart_receive = 0;
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

    pT_drone->d_kp_second_pitch = 0;
    pT_drone->d_kd_second_pitch = 0;
    pT_drone->d_kp_second_roll = 0;
    pT_drone->d_kd_second_roll = 0;
    pT_drone->d_kp_second_yaw = 0;
    pT_drone->d_kd_second_yaw = 0;
    return 0;
}
/**
 * update the pwm value using by test purpose
 */
//int update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    //char *sz_url_post_pwm = "http://128.97.89.181/rest/api/pwm/post/";
    //char arrc_post_data[100];
    /**
     * store the post data
     */
    //sprintf(arrc_post_data, "pwm1=%f&pwm2=%f&pwm3=%f&pwm4=%f", pT_drone->arrd_current_pwm[0], pT_drone->arrd_current_pwm[1], pT_drone->arrd_current_pwm[2], pT_drone->arrd_current_pwm[3]);
    /**
     * http post
     */
    //http_post(sz_url_post_pwm, arrc_post_data);
    //return 0;
//}
/**
 * update the pid_tuning value using by test purpose
 */
int update_T_drone_http_pid_tuning_get(struct T_drone *pT_drone){
    char *sz_url_get_pid_tuning = "http://128.97.89.181/rest/api/pid_tuning/get/";

    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_pid_tuning[9], *pT_json_object_data, *ppT_json_object_pid_second_tuning[6], *pT_json_object_update_time;
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

    n_json_response = json_object_object_get_ex(pT_json_object_data, "kp_second_pitch", &ppT_json_object_pid_second_tuning[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kd_second_pitch", &ppT_json_object_pid_second_tuning[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kp_second_roll", &ppT_json_object_pid_second_tuning[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kd_second_roll", &ppT_json_object_pid_second_tuning[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kp_second_yaw", &ppT_json_object_pid_second_tuning[4]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "kd_second_yaw", &ppT_json_object_pid_second_tuning[5]);
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

    pT_drone->d_kp_second_pitch = json_object_get_double(*(ppT_json_object_pid_second_tuning + 0));
    pT_drone->d_kd_second_pitch = json_object_get_double(*(ppT_json_object_pid_second_tuning + 1));
    pT_drone->d_kp_second_roll = json_object_get_double(*(ppT_json_object_pid_second_tuning + 2));
    pT_drone->d_kd_second_roll = json_object_get_double(*(ppT_json_object_pid_second_tuning + 3));
    pT_drone->d_kp_second_yaw = json_object_get_double(*(ppT_json_object_pid_second_tuning + 4));
    pT_drone->d_kd_second_yaw = json_object_get_double(*(ppT_json_object_pid_second_tuning + 5));
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

/**
 * 0: from edison to beaglebone
 * 1: from beaglebone to edison
 * check https://github.com/peidong/drone/blob/master/Edison/main/edison-bbb-communication-code.md for commands
 */
int communication_with_beaglebone_uart(int n_direction_flag, struct T_drone *pT_drone, int n_command_index, float f_pid_tuning){
    if (n_direction_flag == 0){
        //from edison to beaglebone
        if (n_command_index == 0){
        }else if (n_command_index == 110){
        }else if (n_command_index == 120){
        }else if (n_command_index == 130){
        }else if (n_command_index == 201){
        }else if (n_command_index == 202){
        }else if (n_command_index == 203){
        }else if (n_command_index == 204){
        }else if (n_command_index == 205){
        }else if (n_command_index == 206){
        }else if (n_command_index == 207){
        }else if (n_command_index == 208){
        }else if (n_command_index == 209){
        }else if (n_command_index == 210){
        }else if (n_command_index == 211){
        }else if (n_command_index == 212){
        }else if (n_command_index == 213){
        }else if (n_command_index == 214){
        }else if (n_command_index == 215){
        }else if (n_command_index == 216){
        }else if (n_command_index == 217){
        }else if (n_command_index == 301){
        }else if (n_command_index == 302){
        }else if (n_command_index == 303){
        }else if (n_command_index == 304){
        }else if (n_command_index == 305){
        }else if (n_command_index == 306){
        }else if (n_command_index == 307){
        }else if (n_command_index == 308){
        }else if (n_command_index == 309){
        }else if (n_command_index == 310){
        }else if (n_command_index == 311){
        }else if (n_command_index == 312){
        }else if (n_command_index == 313){
        }else if (n_command_index == 314){
        }else if (n_command_index == 315){
        }else if (n_command_index == 40){
        }else if (n_command_index == 41){
        }else if (n_command_index == 42){
    } else if (n_direction_flag == 1) {
        //from beaglebone to edison
    }
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
