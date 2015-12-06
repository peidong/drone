#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include <unistd.h>/*usleep*/
#include <math.h>//round
#include <time.h>//nanosleep
#include <mraa.h>
#include <stdint.h>

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
int communication_with_beaglebone_uart(int nflag_direction, struct T_drone *pT_drone, int n_command_index, int nflag_receive_success){
    /**
     * check if uart available
     */
    while (pT_drone->nflag_enable_uart != 1){
        usleep(1300);
    }
    pT_drone->nflag_enable_uart = 0;
    mraa_uart_context edison_uart;
    if (nflag_direction == 0){
        //from edison to beaglebone
        edison_uart = mraa_uart_init(0);
        mraa_uart_set_baudrate(edison_uart, 38400);
        if (n_command_index == 0){
            char arrc_message[6] = "~000$";
            arrc_message[5] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 5);
            usleep(1000);
        }else if (n_command_index >= 100 && n_command_index <= 299){
            char arrc_message[6];
            sprintf(arrc_message, "~%d$", n_command_index);
            arrc_message[5] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 5);
            usleep(1000);
        }else if (n_command_index == 301){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 302){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_ki_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 303){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 304){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 305){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_ki_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 306){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 307){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 308){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_ki_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 309){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 310){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_second_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 311){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_second_pitch);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 312){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_second_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 313){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_second_roll);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 314){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kp_second_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index == 315){
            char arrc_message[14];
            sprintf(arrc_message, "~%d%.6f$", n_command_index, pT_drone->d_kd_second_yaw);
            arrc_message[13] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 13);
            usleep(1000);
        }else if (n_command_index >= 40 && n_command_index <= 42){
            char arrc_message[6];
            if (nflag_receive_success == 1){
                sprintf(arrc_message, "~%ds$", n_command_index);
            }else if (nflag_receive_success == 0){
                sprintf(arrc_message, "~%df$", n_command_index);
            }
            arrc_message[5] = '\0';
            mraa_uart_write(edison_uart, arrc_message, 5);
            usleep(1000);
        }
    }else if (nflag_direction == 1) {
        //from beaglebone to edison
    }
    pT_drone->nflag_enable_uart = 1;
    return 0;
}

int main(){
	mraa_init(); // can we put it in the beginning. Avoid repeating definition.
    /**
     * initialize_struct_T_drone
     */
    initialize_struct_T_drone(&g_T_drone_self);
    int n_command_index;
    double d_pid;
    while(1){
    	printf("Please input your command: -1 to exit\n");
    	scanf("%d", &n_command_index);
    	if (n_command_index == -1)
    	{
    		break;
    	}
        if (n_command_index <= 315 && n_command_index >= 301){
            printf("input the pid value\n");
            scanf("%lf", &d_pid);
            if (n_command_index == 301){
                g_T_drone_self.d_kp_pitch = d_pid;
                usleep(1000);
            }else if (n_command_index == 302){
                g_T_drone_self.d_ki_pitch = d_pid;
                usleep(1000);
            }else if (n_command_index == 303){
                g_T_drone_self.d_kd_pitch = d_pid;
                usleep(1000);
            }else if (n_command_index == 304){
                g_T_drone_self.d_kp_roll = d_pid;
                usleep(1000);
            }else if (n_command_index == 305){
                g_T_drone_self.d_ki_roll = d_pid;
                usleep(1000);
            }else if (n_command_index == 306){
                g_T_drone_self.d_kd_roll = d_pid;
                usleep(1000);
            }else if (n_command_index == 307){
                g_T_drone_self.d_kp_yaw = d_pid;
                usleep(1000);
            }else if (n_command_index == 308){
                g_T_drone_self.d_kd_yaw = d_pid;
                usleep(1000);
            }else if (n_command_index == 309){
                g_T_drone_self.d_ki_yaw = d_pid;
                usleep(1000);
            }else if (n_command_index == 310){
                g_T_drone_self.d_kp_second_pitch = d_pid;
                usleep(1000);
            }else if (n_command_index == 311){
                g_T_drone_self.d_kd_second_pitch = d_pid;
                usleep(1000);
            }else if (n_command_index == 312){
                g_T_drone_self.d_kp_second_roll = d_pid;
                usleep(1000);
            }else if (n_command_index == 313){
                g_T_drone_self.d_kd_second_roll = d_pid;
                usleep(1000);
            }else if (n_command_index == 314){
                g_T_drone_self.d_kp_second_yaw = d_pid;
                usleep(1000);
            }else if (n_command_index == 315){
                g_T_drone_self.d_kd_second_yaw = d_pid;
                usleep(1000);
            }
        }
    	communication_with_beaglebone_uart(0, &g_T_drone_self, n_command_index, 0);
	}
	return 0;
}