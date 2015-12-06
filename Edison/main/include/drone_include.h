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
#define PRINT_DEBUG_UART_MESSAGE
#define PRINT_DEBUG_GPS
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
    int nflag_enable_uart;
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
    int n_face_direction;
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
    /**
     * pid variable
     */
    double d_kp_pitch_last_time;
    double d_ki_pitch_last_time;
    double d_kd_pitch_last_time;
    double d_kp_roll_last_time;
    double d_ki_roll_last_time;
    double d_kd_roll_last_time;
    double d_kp_yaw_last_time;
    double d_ki_yaw_last_time;
    double d_kd_yaw_last_time;
    /**
     * second loop pid values
     */
    double d_kp_second_pitch_last_time;
    double d_kd_second_pitch_last_time;
    double d_kp_second_roll_last_time;
    double d_kd_second_roll_last_time;
    double d_kp_second_yaw_last_time;
    double d_kd_second_yaw_last_time;
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
    pT_drone->nflag_enable_uart = 1;
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
    pT_drone->n_face_direction = 0;
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

    pT_drone->d_kp_pitch_last_time = 0;
    pT_drone->d_ki_pitch_last_time = 0;
    pT_drone->d_kd_pitch_last_time = 0;
    pT_drone->d_kp_roll_last_time = 0;
    pT_drone->d_ki_roll_last_time = 0;
    pT_drone->d_kd_roll_last_time = 0;
    pT_drone->d_kp_yaw_last_time = 0;
    pT_drone->d_ki_yaw_last_time = 0;
    pT_drone->d_kd_yaw_last_time = 0;

    pT_drone->d_kp_second_pitch_last_time = 0;
    pT_drone->d_kd_second_pitch_last_time = 0;
    pT_drone->d_kp_second_roll_last_time = 0;
    pT_drone->d_kd_second_roll_last_time = 0;
    pT_drone->d_kp_second_yaw_last_time = 0;
    pT_drone->d_kd_second_yaw_last_time = 0;
    return 0;
}

/**
 * update the gps_ubidots value
 */
int update_T_drone_http_gps_ubidots_post(struct T_drone *pT_drone){
    char *sz_url_post_gps_ubidots = "http://128.97.89.181/rest/api/gps_ubidots/post/";
    char arrc_post_data[100];
    //store the post data
    sprintf(arrc_post_data, "face_direction=%d&latitude=%f&longitude=%f", pT_drone->n_face_direction, pT_drone->d_current_latitude, pT_drone->d_current_longitude);
    //http post
    http_post(sz_url_post_gps_ubidots, arrc_post_data);
    return 0;
}

int process_message(char *arrc_buffer, struct T_drone *pT_drone){
    /**
     * Process the message
     */
    int n_command_index = -1;
    if (arrc_buffer[1] == '4'){
        /**
         * gps
         */
        n_command_index = 4;
#ifdef PRINT_DEBUG_UART_MESSAGE
        printf("gps received\n");
#endif
        char arrc_face_direction[4] = {'\0'};
        char arrc_latitude[12] = {'\0'};
        char arrc_longitude[12] = {'\0'};
        int nflag_load_face_direction = 0;
        int nflag_load_latitude = 0;
        int nflag_load_longitude = 0;
        int n_message_index = 2;
        int n_face_direction_index = 0;
        int n_latitude_index = 0;
        int n_longitude_index = 0;
        while (nflag_load_face_direction == 0 || nflag_load_latitude == 0 || nflag_load_longitude == 0){
            if (nflag_load_face_direction == 0){
                if (arrc_buffer[n_message_index] == '|'){
                    arrc_face_direction[n_face_direction_index] = '\0';
                    nflag_load_face_direction = 1;
                    n_message_index++;
                }else{
                    arrc_face_direction[n_face_direction_index] = arrc_buffer[n_message_index];
                    n_face_direction_index++;
                    n_message_index++;
                    nflag_load_face_direction = 0;
                }
            }else if (nflag_load_latitude == 0){
                if (arrc_buffer[n_message_index] == '|'){
                    arrc_latitude[n_latitude_index] = '\0';
                    nflag_load_latitude = 1;
                    n_message_index++;
                }else{
                    arrc_latitude[n_latitude_index] = arrc_buffer[n_message_index];
                    n_latitude_index++;
                    n_message_index++;
                    nflag_load_latitude = 0;
                }
            }else if (nflag_load_longitude == 0){
                if (arrc_buffer[n_message_index] == '$'){
                    arrc_longitude[n_longitude_index] = '\0';
                    nflag_load_longitude = 1;
                    n_message_index++;
                }else{
                    arrc_longitude[n_longitude_index] = arrc_buffer[n_message_index];
                    n_longitude_index++;
                    n_message_index++;
                    nflag_load_longitude = 0;
                }
            }
        }
        pT_drone->n_face_direction = atoi(arrc_face_direction);
        pT_drone->d_current_latitude = atof(arrc_latitude);
        pT_drone->d_current_longitude = atof(arrc_longitude);
#ifdef PRINT_DEBUG_GPS
        printf("direction:%d\t", pT_drone->n_face_direction);
        printf("lat:%.6f\t", pT_drone->d_current_latitude);
        printf("lng:%.6f\n", pT_drone->d_current_longitude);
#endif
    }
    return 0;
}

/**
 * n_direction_flag: 0 from edison to beaglebone
 *                  1 from beaglebone to edison
 * check https://github.com/peidong/drone/blob/master/Edison/main/edison-bbb-communication-code.md for commands
 */
int communication_with_beaglebone_uart(int nflag_direction, struct T_drone *pT_drone, int n_command_index, int nflag_receive_success){
    /**
     * check if uart available
     */
    while (pT_drone->nflag_enable_uart != 1){
        if (pT_drone->nflag_stop_all != 0){
            return 0;
        }
        usleep(1300);
    }
    pT_drone->nflag_enable_uart = 0;
    mraa_uart_context edison_uart;
    edison_uart = mraa_uart_init(0);
    mraa_uart_set_baudrate(edison_uart, 38400);
    if (nflag_direction == 0){
        //from edison to beaglebone
#ifdef PRINT_DEBUG_UART_MESSAGE
    printf("sending command: %d\n", n_command_index);
#endif
        if (n_command_index == 0){
            char arrc_message[6] = "~000$";
            arrc_message[5] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 5);
            usleep(1000);
        }else if (n_command_index >= 100 && n_command_index <= 299){
            char arrc_message[6] = {'\0'};
            sprintf(arrc_message, "~%d$", n_command_index);
            arrc_message[5] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 5);
            usleep(1000);
        }else if (n_command_index == 301){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 302){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_ki_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 303){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 304){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 305){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_ki_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 306){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 307){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 308){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_ki_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 309){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 310){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_second_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 311){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_second_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 312){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_second_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 313){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_second_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 314){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_second_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 315){
            char arrc_message[14] = {'\0'};
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_second_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index >= 40 && n_command_index <= 42){
            char arrc_message[6] = {'\0'};
            if (nflag_receive_success == 1){
                sprintf(arrc_message, "~%ds$", n_command_index);
            }else if (nflag_receive_success == 0){
                sprintf(arrc_message, "~%df$", n_command_index);
            }
            arrc_message[5] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 5);
            usleep(1000);
        }
    }else if (nflag_direction == 1){
        //from beaglebone to edison
        char c_flag[1] = {'\0'};
        char arrc_buffer[31] = {'\0'};
        int nflag_find_beginning = 0;
        int nflag_find_end = 0;
        int n_receive_message_index = 0;
        /**
         * Read the message array
         */
        printf("1\n");
        while (nflag_find_beginning != 1){
            if (pT_drone->nflag_stop_all != 0){
                break;
            }
            printf("2\n");
            if (mraa_uart_data_available(edison_uart, 100) == 1){
                mraa_uart_read(edison_uart, c_flag, 1);
                printf("3\n");
                if (c_flag[0] == '~'){
                    nflag_find_beginning = 1;
                    n_receive_message_index = 0;
                    arrc_buffer[n_receive_message_index] = '~';
                    while (nflag_find_end != 1){
                        if (pT_drone->nflag_stop_all != 0){
                            break;
                        }
                        if (mraa_uart_data_available(edison_uart, 100) == 1){
                            mraa_uart_read(edison_uart, arrc_buffer + n_receive_message_index, 1);
                            if (arrc_buffer[n_receive_message_index] == '$'){
                                n_receive_message_index++;
                                arrc_buffer[n_receive_message_index] = '\0';
                                nflag_find_end = 1;
                                //break;
                            }else if (arrc_buffer[n_receive_message_index] == '~'){
                                nflag_find_end = -1;
                                nflag_find_beginning = 1;
                                n_receive_message_index = 0;
                                arrc_buffer[n_receive_message_index] = '~';
                                //continue;
                            }else{
                                n_receive_message_index++;
                                nflag_find_end = 0;
                            }
                        }
                    }
                }
            }
        }
        nflag_find_beginning = 0;
        nflag_find_end = 0;
#ifdef PRINT_DEBUG_UART_MESSAGE
    printf("Receive package %s\n", arrc_buffer);
#endif
        process_message(arrc_buffer, pT_drone);
        update_T_drone_http_gps_ubidots_post(pT_drone);
    }
    pT_drone->nflag_enable_uart = 1;
    return 0;
}

/**
 * update the pwm value using by test purpose
 */
//int update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    //char *sz_url_post_pwm = "http://128.97.89.181/rest/api/pwm/post/";
    //char arrc_post_data[100];
    //*
     //store the post data
    //sprintf(arrc_post_data, "pwm1=%f&pwm2=%f&pwm3=%f&pwm4=%f", pT_drone->arrd_current_pwm[0], pT_drone->arrd_current_pwm[1], pT_drone->arrd_current_pwm[2], pT_drone->arrd_current_pwm[3]);
    //*
     //http post
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

    if (pT_drone->d_kp_pitch != pT_drone->d_kp_pitch_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kp_pitch_last_time = pT_drone->d_kp_pitch;
        communication_with_beaglebone_uart(0, pT_drone, 301, -1);
        usleep(100000);
    }
    if (pT_drone->d_ki_pitch != pT_drone->d_ki_pitch_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_ki_pitch_last_time = pT_drone->d_ki_pitch;
        communication_with_beaglebone_uart(0, pT_drone, 302, -1);
        usleep(100000);
    }
    if (pT_drone->d_kd_pitch != pT_drone->d_kd_pitch_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kd_pitch_last_time = pT_drone->d_kd_pitch;
        communication_with_beaglebone_uart(0, pT_drone, 303, -1);
        usleep(100000);
    }
    if (pT_drone->d_kp_roll != pT_drone->d_kp_roll_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kp_roll_last_time = pT_drone->d_kp_roll;
        communication_with_beaglebone_uart(0, pT_drone, 304, -1);
        usleep(100000);
    }
    if (pT_drone->d_ki_roll != pT_drone->d_ki_roll_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_ki_roll_last_time = pT_drone->d_ki_roll;
        communication_with_beaglebone_uart(0, pT_drone, 305, -1);
        usleep(100000);
    }
    if (pT_drone->d_kd_roll != pT_drone->d_kd_roll_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kd_roll_last_time = pT_drone->d_kd_roll;
        communication_with_beaglebone_uart(0, pT_drone, 306, -1);
        usleep(100000);
    }
    if (pT_drone->d_kp_yaw != pT_drone->d_kp_yaw_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kp_yaw_last_time = pT_drone->d_kp_yaw;
        communication_with_beaglebone_uart(0, pT_drone, 307, -1);
        usleep(100000);
    }
    if (pT_drone->d_ki_yaw != pT_drone->d_ki_yaw_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_ki_yaw_last_time = pT_drone->d_ki_yaw;
        communication_with_beaglebone_uart(0, pT_drone, 308, -1);
        usleep(100000);
    }
    if (pT_drone->d_kd_yaw != pT_drone->d_kd_yaw_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kd_yaw_last_time = pT_drone->d_kd_yaw;
        communication_with_beaglebone_uart(0, pT_drone, 309, -1);
        usleep(100000);
    }
    if (pT_drone->d_kp_second_pitch != pT_drone->d_kp_second_pitch_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kp_second_pitch_last_time = pT_drone->d_kp_second_pitch;
        communication_with_beaglebone_uart(0, pT_drone, 310, -1);
        usleep(100000);
    }
    if (pT_drone->d_kd_second_pitch != pT_drone->d_kd_second_pitch_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kd_second_pitch_last_time = pT_drone->d_kd_second_pitch;
        communication_with_beaglebone_uart(0, pT_drone, 311, -1);
        usleep(100000);
    }
    if (pT_drone->d_kp_second_roll != pT_drone->d_kp_second_roll_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kp_second_roll_last_time = pT_drone->d_kp_second_roll;
        communication_with_beaglebone_uart(0, pT_drone, 312, -1);
        usleep(100000);
    }
    if (pT_drone->d_kd_second_roll != pT_drone->d_kd_second_roll_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kd_second_roll_last_time = pT_drone->d_kd_second_roll;
        communication_with_beaglebone_uart(0, pT_drone, 313, -1);
        usleep(100000);
    }
    if (pT_drone->d_kp_second_yaw != pT_drone->d_kp_second_yaw_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kp_second_yaw_last_time = pT_drone->d_kp_second_yaw;
        communication_with_beaglebone_uart(0, pT_drone, 314, -1);
        usleep(100000);
    }
    if (pT_drone->d_kd_second_yaw != pT_drone->d_kd_second_yaw_last_time && pT_drone->nflag_stop_all == 0){
        pT_drone->d_kd_second_yaw_last_time = pT_drone->d_kd_second_yaw;
        communication_with_beaglebone_uart(0, pT_drone, 315, -1);
        usleep(100000);
    }

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
    if (NULL == sz_url_get_control){
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
        communication_with_beaglebone_uart(0, pT_drone, 0, -1);
        /**
         * set the manual control command back to server
         */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }

    if (pT_drone->n_manual_control_command >= 1 && pT_drone->n_manual_control_command <= 9){
        communication_with_beaglebone_uart(0, pT_drone, (200 + pT_drone->n_manual_control_command), -1);
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }

    /**
     * pwm manual change, 11 small up, 12 small down, 13 big up, 14 big down
     */
    if (pT_drone->n_manual_control_command == 11 || pT_drone->n_manual_control_command == 12 || pT_drone->n_manual_control_command == 13 || pT_drone->n_manual_control_command == 14){
        if (pT_drone->n_manual_control_command == 11){
            communication_with_beaglebone_uart(0, pT_drone, 211, -1);
        }else if (pT_drone->n_manual_control_command == 12){
            communication_with_beaglebone_uart(0, pT_drone, 212, -1);
        }else if (pT_drone->n_manual_control_command == 13){
            communication_with_beaglebone_uart(0, pT_drone, 213, -1);
        }else if (pT_drone->n_manual_control_command == 14){
            communication_with_beaglebone_uart(0, pT_drone, 214, -1);
        }
        /**
         * set the manual control command back to server
         */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }

    /**
     * nflag_enable_pwm_pid_ultrasound
     */
    if (pT_drone->n_manual_control_command == 16 || pT_drone->n_manual_control_command == 17){
        if (pT_drone->n_manual_control_command == 16){
            pT_drone->nflag_enable_pwm_pid_ultrasound = 1;
            communication_with_beaglebone_uart(0, pT_drone, 216, -1);
        }else if (pT_drone->n_manual_control_command == 17){
            pT_drone->nflag_enable_pwm_pid_ultrasound = 0;
            communication_with_beaglebone_uart(0, pT_drone, 217, -1);
        }
        /**
         * set the manual control command back to server
         */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=15");
    }
    /**
     * free pointer
     */
    if (sz_url_get_control != NULL){
        free(sz_url_get_control);
        sz_url_get_control = NULL;
    }
    if (sz_url_post_control != NULL){
        free(sz_url_post_control);
        sz_url_post_control = NULL;
    }
    return 0;
}

/**
 * update the drone value for gps
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

    pT_drone->n_face_direction = json_object_get_int(pT_json_object_face_direction);
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
 * Thread Tasks
 */
//void ThreadTask_update_T_drone_http_pwm_get(struct T_drone *pT_drone){
    //while (1){
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}
//#ifdef PRINT_DEBUG_THREAD
        //printf("ThreadTask_update_T_drone_http_pwm_get\n");
//#endif
        //update_T_drone_http_pwm_get(pT_drone);
//#ifdef PRINT_DEBUG_PWM_HTTP_GET
        //printf("pwm1 = %f\t", pT_drone->arrd_current_pwm[0]);
        //printf("pwm2 = %f\t", pT_drone->arrd_current_pwm[1]);
        //printf("pwm3 = %f\t", pT_drone->arrd_current_pwm[2]);
        //printf("pwm4 = %f\n", pT_drone->arrd_current_pwm[3]);
//#endif
        //usleep(50000);
    //}
//}

//void ThreadTask_update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    //while (1){
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}
//#ifdef PRINT_DEBUG_THREAD
        //printf("ThreadTask_update_T_drone_http_pwm_post\n");
//#endif
        //update_T_drone_http_pwm_post(pT_drone);
        //usleep(50000);
    //}
//}

//void ThreadTask_update_T_drone_http_gps_ubidots_post(struct T_drone *pT_drone){
    //while (1){
        //if (pT_drone->nflag_stop_all != 0){
            //break;
        //}
//#ifdef PRINT_DEBUG_THREAD
        //printf("ThreadTask_update_T_drone_http_gps_ubidots_post\n");
//#endif
        //update_T_drone_http_gps_ubidots_post(pT_drone);
        //usleep(1000000);
    //}
//}

void ThreadTask_update_T_drone_http(struct T_drone *pT_drone){
    while (1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http\n");
#endif
        update_T_drone_http(pT_drone);
        usleep(500000);
    }
}

void ThreadTask_update_T_drone_arrn_ultrasound(struct T_drone *pT_drone){
    while (1){
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
    while (1){
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
    while (1){
        if (pT_drone->nflag_stop_all != 0){
            break;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("ThreadTask_update_T_drone_http_pid_tuning_get\n");
#endif
        update_T_drone_http_pid_tuning_get(pT_drone);
        usleep(800000);
    }
}

void ThreadTask_uart_message(struct T_drone *pT_drone){
    while (pT_drone->nflag_stop_all == 0 && pT_drone->nflag_enable_uart == 1){
        communication_with_beaglebone_uart(1, pT_drone, -1, -1);
        usleep(1000000);
    }
}
