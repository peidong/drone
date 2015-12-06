#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include <unistd.h>/*usleep*/
#include <math.h>//round
#include <time.h>//nanosleep
#include <mraa.h>
#include <stdint.h>
#include "bbb/pwm.h"
#include "bbb/uart.h"
#include "bbb/gpio.h"

#define PID_SLEEP_US 50000
#define PWM_DEVIDE_RATIO 1
#define PWM_MANUAL_CHANGE_AMOUNT 0.000025
#define PWM_MANUAL_CHANGE_AMOUNT_LARGE 0.000025*80
#define PWM_INITIAL 0.000025*4
#define PWM_MIN 0.000025*1000
#define PWM_RANGE 0.000025*500

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
    pT_drone->nflag_enable_pwm_pid_ultrasound = 1;
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

    pT_drone->d_kp_second_pitch = 0;
    pT_drone->d_kd_second_pitch = 0;
    pT_drone->d_kp_second_roll = 0;
    pT_drone->d_kd_second_roll = 0;
    pT_drone->d_kp_second_yaw = 0;
    pT_drone->d_kd_second_yaw = 0;
    return 0;
}

/**
 * n_direction_flag: 0 from edison to beaglebone
 *                  1 from beaglebone to edison
 * check https://github.com/peidong/drone/blob/master/Edison/main/edison-bbb-communication-code.md for commands
 */
int communication_with_beaglebone_uart(int nflag_direction, struct T_drone *pT_drone, int nflag_receive_success){
    /**
     * check if uart available
     */
    while (pT_drone->nflag_enable_uart != 1){
        usleep(1300);
    }
    pT_drone->nflag_enable_uart = 0;
    mraa_uart_context beaglebone_uart;
    if (nflag_direction == 1){
        /**
         * From beaglebone to edison
         */
        beaglebone_uart = mraa_uart_init_raw("/dev/ttyO4");
        mraa_uart_set_baudrate(beaglebone_uart, 38400);
        mraa_uart_set_mode(beaglebone_uart, 8, MRAA_UART_PARITY_NONE , 1);
        while (mraa_uart_data_available(beaglebone_uart, 10000) != 1){
            printf("data not available\n");
            /*usleep(10000);*/
        }
        /**
         * Start receive
         */
        char c_flag[1];
        char arrc_buffer[20];
        int nflag_find_beginning = 0;
        int nflag_find_end = 0;
        int n_index = 0;
        /**
         * Read the message array
         */
        while (nflag_find_beginning != 1){
            mraa_uart_read(beaglebone_uart, c_flag, 1);
            if (c_flag[0] == '~'){
                nflag_find_beginning = 1;
                n_index = 0;
                while (nflag_find_end != 1){
                    mraa_uart_read(beaglebone_uart, arrc_buffer + n_index, 1);
                    if (arrc_buffer[n_index] == '$'){
                        arrc_buffer[n_index] = '\0';
                        nflag_find_end = 1;
                        //break;
                    }else if (arrc_buffer[n_index] == '~'){
                        nflag_find_end = -1;
                        nflag_find_beginning = 1;
                        n_index = 0;
                        //continue;
                    }else{
                        n_index++;
                    }
                }
            }
        }
         /**
         * Process the message
         */
        int n_command_index = -1;
        if (arrc_buffer[0] == '0'){
            /**
             * stop
             */
            n_command_index = 0;
            printf("stop received\n");
        }else if (arrc_buffer[0] == '1'){
            /**
             * auto control
             */
            char arrc_command_index[4];
            int n_temp_index;
            for (n_temp_index = 0; n_temp_index <= 2; n_temp_index++){
                arrc_command_index[n_temp_index] = arrc_buffer[n_temp_index];
            }
            arrc_command_index[3] = '\0';
            n_command_index = atoi(arrc_command_index);
            printf("auto control received: %d\n", n_command_index);
        }else if (arrc_buffer[0] == '2'){
            /**
             * manual control command
             */
            char arrc_command_index[4];
            int n_temp_index;
            for (n_temp_index = 0; n_temp_index <= 2; n_temp_index++){
                arrc_command_index[n_temp_index] = arrc_buffer[n_temp_index];
            }
            arrc_command_index[3] = '\0';
            n_command_index = atoi(arrc_command_index);
            printf("manual control received: %d\n", n_command_index);
        }else if (arrc_buffer[0] == '3'){
            /**
             * pid tuning
             */
            char arrc_command_index[4];
            int n_temp_index;
            for (n_temp_index = 0; n_temp_index <= 2; n_temp_index++){
                arrc_command_index[n_temp_index] = arrc_buffer[n_temp_index];
            }
            arrc_command_index[3] = '\0';
            n_command_index = atoi(arrc_command_index);
            printf("pid tuning received: %d\n", n_command_index);

            char arrc_pid_value[9];
            for (n_temp_index = 0; n_temp_index <= 7; n_temp_index++){
                arrc_pid_value[n_temp_index] = arrc_buffer[n_temp_index + 3];
            }
            arrc_buffer[8] = '\0';
            if (n_command_index == 301){
                pT_drone->d_kp_pitch = atof(arrc_pid_value);
            }else if (n_command_index == 302){
                pT_drone->d_ki_pitch = atof(arrc_pid_value);
            }else if (n_command_index == 303){
                pT_drone->d_kd_pitch = atof(arrc_pid_value);
            }else if (n_command_index == 304){
                pT_drone->d_kp_roll = atof(arrc_pid_value);
            }else if (n_command_index == 305){
                pT_drone->d_ki_roll = atof(arrc_pid_value);
            }else if (n_command_index == 306){
                pT_drone->d_kd_roll = atof(arrc_pid_value);
            }else if (n_command_index == 307){
                pT_drone->d_kp_yaw = atof(arrc_pid_value);
            }else if (n_command_index == 308){
                pT_drone->d_kd_yaw = atof(arrc_pid_value);
            }else if (n_command_index == 309){
                pT_drone->d_ki_yaw = atof(arrc_pid_value);
            }else if (n_command_index == 310){
                pT_drone->d_kp_second_pitch = atof(arrc_pid_value);
            }else if (n_command_index == 311){
                pT_drone->d_kd_second_pitch = atof(arrc_pid_value);
            }else if (n_command_index == 312){
                pT_drone->d_kp_second_roll = atof(arrc_pid_value);
            }else if (n_command_index == 313){
                pT_drone->d_kd_second_roll = atof(arrc_pid_value);
            }else if (n_command_index == 314){
                pT_drone->d_kp_second_yaw = atof(arrc_pid_value);
            }else if (n_command_index == 315){
                pT_drone->d_kd_second_yaw = atof(arrc_pid_value);
            }
            printf("pid tuning value: %lf\n", atof(arrc_pid_value));
        }else if (arrc_buffer[0] == '4'){
            /**
             * feedback
             */
            printf("feedback received\n");
        }
    }else if (nflag_direction == 0){
        /**
         * From edison to beaglebone
         */
    }
    pT_drone->nflag_enable_uart = 1;
    return 0;
}

int main()
{
	load_device_tree("ADAFRUIT-UART4");
    initialize_struct_T_drone(&g_T_drone_self);
    while(1){
    	communication_with_beaglebone_uart(1, &g_T_drone_self, 0);
    }
	return 0;
}
