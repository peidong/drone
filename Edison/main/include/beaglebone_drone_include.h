#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "thread/thpool.h" /*https://github.com/Pithikos/C-Thread-Pool*/
#include <unistd.h>/*usleep*/
#include <math.h>//round
#include <time.h>//nanosleep
#define MPU9250_UART
// #define MPU9250_I2C
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
#include "bbb/pwm.h"
/**
 * print debug
 */
// #define PRINT_DEBUG_PWM_HTTP_GET
#define PRINT_DEBUG_YAW_PITCH_ROLL
#define PRINT_DEBUG_PID_CHANGE
// #define PRINT_DEBUG_PID_TUNING
// #define PRINT_DEBUG_PWM
#define PRINT_DEBUG_THREAD
// #define PRINT_CAR_MANUAL
//#define TIMER
//#define TIMER_YAW_PITCH_ROLL
//#define TIMER_PID

/**
 * define value
 */
#define PID_SLEEP_US 50000
#define PWM_DEVIDE_RATIO 1
#define PWM_MANUAL_CHANGE_AMOUNT 0.000025
#define PWM_MANUAL_CHANGE_AMOUNT_LARGE 0.000025*80
#define PWM_INITIAL 0.000025*4
#define PWM_MIN 0.000025*1000
#define PWM_RANGE 0.000025*500

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

            // grawy -= 30;

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
            if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
                //n_index_yaw_pitch_roll++;
                //n_index_yaw_pitch_roll = n_index_yaw_pitch_roll%10;
                //if(n_index_yaw_pitch_roll == 0){
                    printf("yaw = %.1f\tpitch = %.1f\troll = %.1f\n",yaw, pitch, roll);
                //}
            }
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
#ifdef TIMER_PID
    timer_start(&g_timer);
#endif
    pidData_t *pidData_yaw = NULL, *pidData_pitch = NULL, *pidData_roll = NULL;
    pidData_t *pidData_second_yaw = NULL, *pidData_second_pitch = NULL, *pidData_second_roll = NULL;
    if(NULL == pidData_yaw){
        pidData_yaw = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_pitch){
        pidData_pitch = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_roll){
        pidData_roll = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_second_yaw){
        pidData_second_yaw = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_second_pitch){
        pidData_second_pitch = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_second_roll){
        pidData_second_roll = (pidData_t*) malloc(sizeof(pidData_t));
    }

    double kp_pitch, ki_pitch, kd_pitch, kp_roll, ki_roll, kd_roll, kp_yaw, ki_yaw, kd_yaw;
    double kp_second_pitch, kd_second_pitch, kp_second_roll, kd_second_roll, kp_second_yaw, kd_second_yaw;
    double d_rate_pitch, d_rate_roll, d_rate_yaw;
    double d_second_yaw, d_second_pitch, d_second_roll;
    ctrlDir_t controllerDir;
    uint32_t samplePeriodMs;
    //int n_index;

    // For the nine values, if we can modify them in IOS app, tests can be easier!
    kp_pitch = pT_drone->d_kp_pitch;
    ki_pitch = pT_drone->d_ki_pitch;
    kd_pitch = pT_drone->d_kd_pitch;
    kp_roll = pT_drone->d_kp_roll;
    ki_roll = pT_drone->d_ki_roll;
    kd_roll = pT_drone->d_kd_roll;
    kp_yaw  = pT_drone->d_kp_yaw;
    ki_yaw  = pT_drone->d_ki_yaw;
    kd_yaw  = pT_drone->d_kd_yaw;
    //six second loop values
    kp_second_pitch = pT_drone->d_kp_second_pitch;
    kd_second_pitch = pT_drone->d_kd_second_pitch;
    kp_second_roll = pT_drone->d_kp_second_roll;
    kd_second_roll = pT_drone->d_kd_second_roll;
    kp_second_yaw = pT_drone->d_kp_second_yaw;
    kd_second_yaw = pT_drone->d_kd_second_yaw;

    samplePeriodMs = 50; //need to be setup
    controllerDir = PID_DIRECT; //Direct control not reverse.

    Pid_Init(pidData_yaw, kp_yaw, ki_yaw, kd_yaw, controllerDir, samplePeriodMs);

    Pid_Init(pidData_pitch, kp_pitch, ki_pitch, kd_pitch, controllerDir, samplePeriodMs);

    Pid_Init(pidData_roll, kp_roll, ki_roll, kd_roll, controllerDir, samplePeriodMs);

    Pid_Init(pidData_second_yaw, kp_second_yaw, 0, kd_second_yaw, controllerDir, samplePeriodMs);

    Pid_Init(pidData_second_pitch, kp_second_pitch, 0, kd_second_pitch, controllerDir, samplePeriodMs);

    Pid_Init(pidData_second_roll, kp_second_roll, 0, kd_second_roll, controllerDir, samplePeriodMs);
    while(1){
#ifdef TIMER_PID
        g_last_time_us = timer_delta_us(&g_timer);
        timer_unpause(&g_timer);
#endif
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("Pid Thread\n");
#endif
        kp_pitch = pT_drone->d_kp_pitch;
        ki_pitch = pT_drone->d_ki_pitch;
        kd_pitch = pT_drone->d_kd_pitch;
        kp_roll = pT_drone->d_kp_roll;
        ki_roll = pT_drone->d_ki_roll;
        kd_roll = pT_drone->d_kd_roll;
        kp_yaw  = pT_drone->d_kp_yaw;
        ki_yaw  = pT_drone->d_ki_yaw;
        kd_yaw  = pT_drone->d_kd_yaw;
        kp_second_pitch = pT_drone->d_kp_second_pitch;
        kd_second_pitch = pT_drone->d_kd_second_pitch;
        kp_second_roll = pT_drone->d_kp_second_roll;
        kd_second_roll = pT_drone->d_kd_second_roll;
        kp_second_yaw = pT_drone->d_kp_second_yaw;
        kd_second_yaw = pT_drone->d_kd_second_yaw;
#ifdef PRINT_DEBUG_PID_TUNING
        printf("p_pitch = %f\t", pT_drone->d_kp_pitch);
        printf("i_pitch = %f\t", pT_drone->d_ki_pitch);
        printf("d_pitch = %f\t", pT_drone->d_kd_pitch);
        printf("p_roll = %f\t", pT_drone->d_kp_roll);
        printf("i_roll = %f\t", pT_drone->d_ki_roll);
        printf("d_roll = %f\t", pT_drone->d_kd_roll);
        printf("p_yaw = %f\t", pT_drone->d_kp_yaw);
        printf("i_yaw = %f\t", pT_drone->d_ki_yaw);
        printf("d_yaw = %f\t", pT_drone->d_kd_yaw);
        printf("p_second_pitch = %f\t", pT_drone->d_kp_second_pitch);
        printf("d_second_pitch = %f\t", pT_drone->d_kd_second_pitch);
        printf("p_second_roll = %f\t", pT_drone->d_kp_second_roll);
        printf("d_second_roll = %f\t", pT_drone->d_kd_second_roll);
        printf("p_second_yaw = %f\t", pT_drone->d_kp_second_yaw);
        printf("d_second_yaw = %f\n", pT_drone->d_kd_second_yaw);
#endif
        Pid_SetTunings(pidData_yaw, kp_yaw, ki_yaw, kd_yaw);
        Pid_SetTunings(pidData_pitch, kp_pitch, ki_pitch, kd_pitch);
        Pid_SetTunings(pidData_roll, kp_roll, ki_roll, kd_roll);

        //"0" is the setpoint or the destination of the final attitude, representing hovering or suspending.
        //Replace "0" by HTTP request parameters later.

        // It can be tested after tests for pitch and roll are finished.
        Pid_SetSetPoint(pidData_yaw, 0);
        Pid_Run(pidData_yaw, (int)pT_drone->arrd_yaw_pitch_roll[0], 0);
        pT_drone->arrd_pid_yaw_pitch_roll[0] = pidData_yaw->output;

        // For pitch, mainly we can use wires to lock the Y direction. First divide by 2. Adding to pwm1 and pwm2, substracting to pwm3 and pwm4.
        Pid_SetSetPoint(pidData_pitch, 0);
        Pid_Run(pidData_pitch, (int)pT_drone->arrd_yaw_pitch_roll[1], 0);
        pT_drone->arrd_pid_yaw_pitch_roll[1] = pidData_pitch->output;
        // For roll, mainly we can use wires to lock the X direction. First divide by 2. Adding to pwm1 and pwm3, substracting to pwm2 and pwm4.
        Pid_SetSetPoint(pidData_roll, 0);
        Pid_Run(pidData_roll, (int)pT_drone->arrd_yaw_pitch_roll[2], 0);
        pT_drone->arrd_pid_yaw_pitch_roll[2] = pidData_roll->output;

        //second loop
        d_rate_yaw = pidData_yaw->output/1000.0;
        d_rate_pitch = pidData_pitch->output/1000.0;
        d_rate_roll = pidData_roll->output/1000.0;
        // d_rate_yaw = 0;
        // d_rate_pitch = 0;
        // d_rate_roll = 0.01;

        Pid_SetTunings(pidData_second_yaw, kp_second_yaw, 0, kd_second_yaw);
        Pid_SetTunings(pidData_second_pitch, kp_second_pitch, 0, kd_second_pitch);
        Pid_SetTunings(pidData_second_roll, kp_second_roll, 0, kd_second_roll);

        Pid_SetSetPoint(pidData_second_yaw, d_rate_yaw);
        Pid_SetSetPoint(pidData_second_pitch, d_rate_pitch);
        Pid_SetSetPoint(pidData_second_roll, d_rate_roll);

        Pid_Run(pidData_second_yaw, pT_drone->n_grawz/32768.0, 0);
        Pid_Run(pidData_second_pitch, pT_drone->n_grawy/32768.0, 0);
        Pid_Run(pidData_second_roll, pT_drone->n_grawx/32768.0 - 0.0009, 1);

        d_second_yaw = pidData_second_yaw->output;
        d_second_pitch = pidData_second_pitch->output;
        d_second_roll = pidData_second_roll->output;

        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }else{
            pT_drone->arrd_current_pwm[0] += (d_second_yaw / 2);
            pT_drone->arrd_current_pwm[1] -= (d_second_yaw / 2);
            pT_drone->arrd_current_pwm[2] += (d_second_yaw / 2);
            pT_drone->arrd_current_pwm[3] -= (d_second_yaw / 2);
        }

        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }else{
            pT_drone->arrd_current_pwm[0] += (d_second_pitch / 2);
            pT_drone->arrd_current_pwm[1] += (d_second_pitch / 2);
            pT_drone->arrd_current_pwm[2] -= (d_second_pitch / 2);
            pT_drone->arrd_current_pwm[3] -= (d_second_pitch / 2);
        }

        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }else{
            pT_drone->arrd_current_pwm[0] -= (d_second_roll / 200);
            pT_drone->arrd_current_pwm[1] += (d_second_roll / 200);
            pT_drone->arrd_current_pwm[2] += (d_second_roll / 200);
            pT_drone->arrd_current_pwm[3] -= (d_second_roll / 200);
        }
        /**
         * change pwm to PWM_DEFAULT_VALUE if below 0
         */
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }
        int n_index;
        for(n_index = 0; n_index < 4; n_index++){
            if(pT_drone->arrd_current_pwm[n_index] > (pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE)){
                pT_drone->arrd_current_pwm[n_index] = (pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE);
            }
            if(pT_drone->arrd_current_pwm[n_index] < pT_drone->arrd_current_pwm_min[n_index]){
                pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index];
            }
        }
#ifdef  PRINT_DEBUG_PID_CHANGE
        printf("first roll= %f\tsecond roll= %f\traw= %f\n",(d_rate_roll), (d_second_roll/200), pT_drone->n_grawx/32768.0 - 0.0009);
#endif
        usleep(PID_SLEEP_US); // We need to add some delay to slow down the pid loop. Mainly, 100ms cycle should be good.
#ifdef TIMER_PID
        timer_pause(&g_timer);
        printf("Delta (us): %ld\n", timer_delta_us(&g_timer) - g_last_time_us);
#endif
    }
    /**
     * free pointer
     */
    if(pidData_yaw != NULL){
        free(pidData_yaw);
        pidData_yaw = NULL;
    }
    if(pidData_pitch != NULL){
        free(pidData_pitch);
        pidData_pitch = NULL;
    }
    if(pidData_roll != NULL){
        free(pidData_roll);
        pidData_roll = NULL;
    }
    if(pidData_second_yaw != NULL){
        free(pidData_second_yaw);
        pidData_second_yaw = NULL;
    }
    if(pidData_second_pitch != NULL){
        free(pidData_second_pitch);
        pidData_second_pitch = NULL;
    }
    if(pidData_second_roll != NULL){
        free(pidData_second_roll);
        pidData_second_roll = NULL;
    }
    return 0;
}
int update_T_drone_arrd_pid_one_loop(struct T_drone *pT_drone){
#ifdef TIMER_PID
    timer_start(&g_timer);
#endif
    pidData_t *pidData_yaw = NULL, *pidData_pitch = NULL, *pidData_roll = NULL;
    if(NULL == pidData_yaw){
        pidData_yaw = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_pitch){
        pidData_pitch = (pidData_t*) malloc(sizeof(pidData_t));
    }
    if(NULL == pidData_roll){
        pidData_roll = (pidData_t*) malloc(sizeof(pidData_t));
    }

    double kp_pitch, ki_pitch, kd_pitch, kp_roll, ki_roll, kd_roll, kp_yaw, ki_yaw, kd_yaw;
    ctrlDir_t controllerDir;
    uint32_t samplePeriodMs;
    //int n_index;

    // For the nine values, if we can modify them in IOS app, tests can be easier!
    kp_pitch = pT_drone->d_kp_pitch;
    ki_pitch = pT_drone->d_ki_pitch;
    kd_pitch = pT_drone->d_kd_pitch;
    kp_roll = pT_drone->d_kp_roll;
    ki_roll = pT_drone->d_ki_roll;
    kd_roll = pT_drone->d_kd_roll;
    kp_yaw  = pT_drone->d_kp_yaw;
    ki_yaw  = pT_drone->d_ki_yaw;
    kd_yaw  = pT_drone->d_kd_yaw;

    samplePeriodMs = 50; //need to be setup
    controllerDir = PID_DIRECT; //Direct control not reverse.

    Pid_Init(pidData_yaw, kp_yaw, ki_yaw, kd_yaw, controllerDir, samplePeriodMs);

    Pid_Init(pidData_pitch, kp_pitch, ki_pitch, kd_pitch, controllerDir, samplePeriodMs);

    Pid_Init(pidData_roll, kp_roll, ki_roll, kd_roll, controllerDir, samplePeriodMs);

    while(1){
#ifdef TIMER_PID
        g_last_time_us = timer_delta_us(&g_timer);
        timer_unpause(&g_timer);
#endif
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }
#ifdef PRINT_DEBUG_THREAD
        printf("Pid Thread\n");
#endif
        kp_pitch = pT_drone->d_kp_pitch;
        ki_pitch = pT_drone->d_ki_pitch;
        kd_pitch = pT_drone->d_kd_pitch;
        kp_roll = pT_drone->d_kp_roll;
        ki_roll = pT_drone->d_ki_roll;
        kd_roll = pT_drone->d_kd_roll;
        kp_yaw  = pT_drone->d_kp_yaw;
        ki_yaw  = pT_drone->d_ki_yaw;
        kd_yaw  = pT_drone->d_kd_yaw;
#ifdef PRINT_DEBUG_PID_TUNING
        printf("p_pitch = %f\t", pT_drone->d_kp_pitch);
        printf("i_pitch = %f\t", pT_drone->d_ki_pitch);
        printf("d_pitch = %f\t", pT_drone->d_kd_pitch);
        printf("p_roll = %f\t", pT_drone->d_kp_roll);
        printf("i_roll = %f\t", pT_drone->d_ki_roll);
        printf("d_roll = %f\t", pT_drone->d_kd_roll);
        printf("p_yaw = %f\t", pT_drone->d_kp_yaw);
        printf("i_yaw = %f\t", pT_drone->d_ki_yaw);
        printf("d_yaw = %f\n", pT_drone->d_kd_yaw);
#endif
        Pid_SetTunings(pidData_yaw, kp_yaw, ki_yaw, kd_yaw);
        Pid_SetTunings(pidData_pitch, kp_pitch, ki_pitch, kd_pitch);
        Pid_SetTunings(pidData_roll, kp_roll, ki_roll, kd_roll);

        //"0" is the setpoint or the destination of the final attitude, representing hovering or suspending.
        //Replace "0" by HTTP request parameters later.

        // It can be tested after tests for pitch and roll are finished.
        Pid_SetSetPoint(pidData_yaw, 0);
        Pid_Run(pidData_yaw, pT_drone->arrd_yaw_pitch_roll[0], 0);
        pT_drone->arrd_pid_yaw_pitch_roll[0] = pidData_yaw->output;

        // For pitch, mainly we can use wires to lock the Y direction. First divide by 2. Adding to pwm1 and pwm2, substracting to pwm3 and pwm4.
        Pid_SetSetPoint(pidData_pitch, 0);
        Pid_Run(pidData_pitch, (int)pT_drone->arrd_yaw_pitch_roll[1], 0);
        pT_drone->arrd_pid_yaw_pitch_roll[1] = pidData_pitch->output;
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }else{
            pT_drone->arrd_current_pwm[0] += (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
            pT_drone->arrd_current_pwm[1] += (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);

            pT_drone->arrd_current_pwm[2] -= (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
            pT_drone->arrd_current_pwm[3] -= (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
        }
        // For roll, mainly we can use wires to lock the X direction. First divide by 2. Adding to pwm1 and pwm3, substracting to pwm2 and pwm4.
        Pid_SetSetPoint(pidData_roll, 0);
        Pid_Run(pidData_roll, (int)pT_drone->arrd_yaw_pitch_roll[2], 0);
        pT_drone->arrd_pid_yaw_pitch_roll[2] = pidData_roll->output;
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }else{
            pT_drone->arrd_current_pwm[0] -= (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
            pT_drone->arrd_current_pwm[3] -= (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);

            pT_drone->arrd_current_pwm[1] += (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
            pT_drone->arrd_current_pwm[2] += (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
        }
        /**
         * change pwm to PWM_DEFAULT_VALUE if below 0
         */
        if (pT_drone->nflag_stop_all != 0){
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            usleep(PID_SLEEP_US);
            continue;
        }
        int n_index;
        for(n_index = 0; n_index < 4; n_index++){
            if(pT_drone->arrd_current_pwm[n_index] > (pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE)){
                pT_drone->arrd_current_pwm[n_index] = (pT_drone->arrd_current_pwm_min[n_index] + PWM_RANGE);
            }
            if(pT_drone->arrd_current_pwm[n_index] < pT_drone->arrd_current_pwm_min[n_index]){
                pT_drone->arrd_current_pwm[n_index] = pT_drone->arrd_current_pwm_min[n_index];
            }
        }
#ifdef  PRINT_DEBUG_PID_CHANGE
        printf("pitch change= %f\troll change= %f\n",(pT_drone->arrd_pid_yaw_pitch_roll[1] / 2), (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2));
#endif
        usleep(PID_SLEEP_US); // We need to add some delay to slow down the pid loop. Mainly, 100ms cycle should be good.
#ifdef TIMER_PID
        timer_pause(&g_timer);
        printf("Delta (us): %ld\n", timer_delta_us(&g_timer) - g_last_time_us);
#endif
    }
    /**
     * free pointer
     */
    if(pidData_yaw != NULL){
        free(pidData_yaw);
        pidData_yaw = NULL;
    }
    if(pidData_pitch != NULL){
        free(pidData_pitch);
        pidData_pitch = NULL;
    }
    if(pidData_roll != NULL){
        free(pidData_roll);
        pidData_roll = NULL;
    }
    return 0;
}

/**
 * Generate pwm wave from beaglebone black
 */
int GeneratePwm(struct T_drone *pT_drone){
    /**
     * Initialize PWM
     */
    pwm_start("P9_14", pT_drone->arrd_current_pwm[0], 50, 0);//pwm3
    pwm_start("P9_16", pT_drone->arrd_current_pwm[1], 50, 0);//pwm4
    pwm_start("P8_13", pT_drone->arrd_current_pwm[2], 50, 0);//pwm6
    pwm_start("P8_19", pT_drone->arrd_current_pwm[3], 50, 0);//pwm5
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
            pwm_set_duty_cycle("P9_14", pT_drone->arrd_current_pwm[0]);
            pwm_set_duty_cycle("P9_16", pT_drone->arrd_current_pwm[1]);
            pwm_set_duty_cycle("P8_13", pT_drone->arrd_current_pwm[2]);
            pwm_set_duty_cycle("P8_19", pT_drone->arrd_current_pwm[3]);
            break;
        }
        else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            pwm_set_duty_cycle("P9_14", pT_drone->arrd_current_pwm[0]);
            pwm_set_duty_cycle("P9_16", pT_drone->arrd_current_pwm[1]);
            pwm_set_duty_cycle("P8_13", pT_drone->arrd_current_pwm[2]);
            pwm_set_duty_cycle("P8_19", pT_drone->arrd_current_pwm[3]);
            continue;
        }
        usleep(2000);//2ms motor modulation frequency max = 490Hz

        /**
         * write pwm duty cycle
         */
        if (pT_drone->nflag_stop_all != 0){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            pwm_set_duty_cycle("P9_14", pT_drone->arrd_current_pwm[0]);
            pwm_set_duty_cycle("P9_16", pT_drone->arrd_current_pwm[1]);
            pwm_set_duty_cycle("P8_13", pT_drone->arrd_current_pwm[2]);
            pwm_set_duty_cycle("P8_19", pT_drone->arrd_current_pwm[3]);
            break;
        }else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            /**
             * Reset PWM to 0
             */
            initialize_pwm_value(pT_drone);
            pwm_set_duty_cycle("P9_14", pT_drone->arrd_current_pwm[0]);
            pwm_set_duty_cycle("P9_16", pT_drone->arrd_current_pwm[1]);
            pwm_set_duty_cycle("P8_13", pT_drone->arrd_current_pwm[2]);
            pwm_set_duty_cycle("P8_19", pT_drone->arrd_current_pwm[3]);
            continue;
        }else{
            pwm_set_duty_cycle("P9_14", pT_drone->arrd_current_pwm[0]);
            pwm_set_duty_cycle("P9_16", pT_drone->arrd_current_pwm[1]);
            pwm_set_duty_cycle("P8_13", pT_drone->arrd_current_pwm[2]);
            pwm_set_duty_cycle("P8_19", pT_drone->arrd_current_pwm[3]);
        }
    }
    /**
     * Reset PWM to 0
     */
    initialize_pwm_value(pT_drone);
    pwm_set_duty_cycle("P9_14", pT_drone->arrd_current_pwm[0]);
    pwm_set_duty_cycle("P9_16", pT_drone->arrd_current_pwm[1]);
    pwm_set_duty_cycle("P8_13", pT_drone->arrd_current_pwm[2]);
    pwm_set_duty_cycle("P8_19", pT_drone->arrd_current_pwm[3]);
    pwm_disable("P9_14");
    pwm_disable("P9_16");
    pwm_disable("P8_13");
    pwm_disable("P8_19");
    pwm_cleanup();
    return 0;
}

/**
 * Thread Tasks
 */

void ThreadTask_update_T_drone_arrd_pid(struct T_drone *pT_drone){
    update_T_drone_arrd_pid(pT_drone);
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
