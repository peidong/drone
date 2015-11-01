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
#include "mpu9250/mpu9250.h"  //include pid file    
#include "pid/pid.h"  //include pid file
//#include "timer/timer.h" //timer
#include <mraa.h>
#include <stdint.h>

#define PWM_PERIOD_NS 20000000//20ms
//#define PWM_PERIOD_NS 5000000000//5s
//#define DEBUG_PWM
//#define DEBUG_GPIO_PWM
#define PWM_DEVIDE_RATIO 1
// #define DEBUG_YAW_PITCH_ROLL
#define PWM_MANUAL_CHANGE_AMOUNT 0.000025

struct T_drone{

    int n_stop_sign;

    //These following are from server
    char *sz_mac_address;
    int n_control_type;
    int n_auto_control_command;
    int n_manual_control_command;
    double arrd_suspend_pwm[4];

    //These following are from the board itself
    double arrd_current_pwm[4];
    //double arrd_last_pwm[4];
    double arrd_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
    double arrd_pid_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
    int arrn_ultrasound[6];/*0:up 1:down 2:left 3:right 4:forward 5:backward*/

    //GPS info
    double d_current_latitude;
    double d_current_longitude;
    double d_face_direction;
    double d_destination_latitude;
    double d_destination_longitude;
};

struct T_drone g_T_drone_self;

int initialize_struct_T_drone(struct T_drone *pT_drone){
    pT_drone->n_stop_sign = 0;
    pT_drone->sz_mac_address = "fc:c2:de:3d:7f:af";

    pT_drone->n_control_type = -1;
    pT_drone->n_auto_control_command = -1;
    pT_drone->n_manual_control_command = -1;

    pT_drone->arrd_suspend_pwm[0] = 0;
    pT_drone->arrd_suspend_pwm[1] = 0;
    pT_drone->arrd_suspend_pwm[2] = 0;
    pT_drone->arrd_suspend_pwm[3] = 0;

    pT_drone->arrd_current_pwm[0] = 0;
    pT_drone->arrd_current_pwm[1] = 0;
    pT_drone->arrd_current_pwm[2] = 0;
    pT_drone->arrd_current_pwm[3] = 0;

    pT_drone->arrd_yaw_pitch_roll[0] = 0;
    pT_drone->arrd_yaw_pitch_roll[1] = 0;
    pT_drone->arrd_yaw_pitch_roll[2] = 0;

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
}

/**
 * update the pwm value using by test purpose
 */
int update_T_drone_http_pwm_get(struct T_drone *pT_drone){
    char *sz_url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    
    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_pwm[4], *pT_json_object_data, *pT_json_object_update_time;
    int n_json_response;
    int n_index=0;

    sz_http_response = http_get(sz_url_get_pwm);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm1", &ppT_json_object_pwm[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm2", &ppT_json_object_pwm[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm3", &ppT_json_object_pwm[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm4", &ppT_json_object_pwm[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "update_time", &pT_json_object_update_time);

    n_index = 0;
    for(n_index = 0; n_index < 4; n_index++)
    {
        pT_drone->arrd_current_pwm[n_index] = (json_object_get_double(*(ppT_json_object_pwm+n_index))) / PWM_DEVIDE_RATIO;
    }
    return 0;
}

/**
 * 
 * update the pwm value using by test purpose
 * 
 */
int update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    char *sz_url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";
    char arrc_post_data[100];

    sprintf(arrc_post_data, "pwm1=%f&pwm2=%f&pwm3=%f&pwm4=%f", pT_drone->arrd_current_pwm[0], pT_drone->arrd_current_pwm[1], pT_drone->arrd_current_pwm[2], pT_drone->arrd_current_pwm[3]);

    http_post(sz_url_post_pwm, arrc_post_data);

    return 0;
}

/**
 * update the drone value
 */
int update_T_drone_http(struct T_drone *pT_drone){
    //How to concat two char* string in C program
    //http://stackoverflow.com/questions/18468229/how-to-concat-two-char-string-in-c-program
    //sz_mac_address = "fc:c2:de:3d:7f:af";
    char *sz_url_get_control_part1 = "http://fryer.ee.ucla.edu/rest/api/control/get/?mac_address=";
    char *sz_url_post_control_part1 = "http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=";
    char *sz_url_get_control = (char*) malloc(1 + strlen(sz_url_get_control_part1) + strlen(pT_drone->sz_mac_address));
    strcpy(sz_url_get_control, sz_url_get_control_part1);
    strcat(sz_url_get_control, pT_drone->sz_mac_address);
    
    char *sz_url_post_control = (char*) malloc(1 + strlen(sz_url_post_control_part1) + strlen(pT_drone->sz_mac_address));
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
    for(n_index = 0; n_index < 4; n_index++)
    {
        pT_drone->arrd_suspend_pwm[n_index] = json_object_get_double(*(ppT_json_object_suspend_pwm + n_index));
    }

/**
 * stop
 */
    if (pT_drone->n_manual_control_command == 10)
    {
        pT_drone->n_stop_sign = 1;
    }

/**
 * pwm manual change
 */
    if (pT_drone->n_manual_control_command == 11 || pT_drone->n_manual_control_command == 12)
    {
        if (pT_drone->n_manual_control_command == 11)
        {
            for(n_index=0; n_index<4; n_index++){
                pT_drone->arrd_current_pwm[n_index] += PWM_MANUAL_CHANGE_AMOUNT;
            }
        }else if (pT_drone->n_manual_control_command == 12)
        {
            for(n_index=0; n_index<4; n_index++){
                pT_drone->arrd_current_pwm[n_index] -= PWM_MANUAL_CHANGE_AMOUNT;
            }
        }
/**
 * set the manual control command back to server
 */
        sz_http_response = http_post(sz_url_post_control, "manual_control_command=-1");
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
    char *sz_url_get_gps_destination = "http://fryer.ee.ucla.edu/rest/api/gps/get/?location_type=0";//get destination gps
    char *sz_url_get_gps_iPhone = "http://fryer.ee.ucla.edu/rest/api/gps/get/?location_type=1";//get iPhone(Edison board) current gps
    char *sz_url_post_gps = "http://fryer.ee.ucla.edu/rest/api/gps/post/?location_type=1";
    
    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *pT_json_object_data, *pT_json_object_update_time, *pT_json_object_face_direction, *pT_json_object_latitude, *pT_json_object_longitude, *pT_json_object_stop_sign;
    int n_json_response;

    sz_http_response = http_get(sz_url_get_gps_iPhone);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "face_direction", &pT_json_object_face_direction);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "latitude", &pT_json_object_latitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "longitude", &pT_json_object_longitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"update_time",&pT_json_object_update_time);

    pT_drone->d_face_direction = json_object_get_double(pT_json_object_face_direction);
    pT_drone->d_current_latitude = json_object_get_double(pT_json_object_latitude);
    pT_drone->d_current_longitude = json_object_get_double(pT_json_object_longitude);

    sz_http_response = http_get(sz_url_get_gps_destination);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "face_direction", &pT_json_object_face_direction);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "latitude", &pT_json_object_latitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "longitude", &pT_json_object_longitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "stop_sign", &pT_json_object_stop_sign);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"update_time",&pT_json_object_update_time);

    pT_drone->d_destination_latitude = json_object_get_double(pT_json_object_latitude);
    pT_drone->d_destination_longitude = json_object_get_double(pT_json_object_longitude);
    pT_drone->n_stop_sign = json_object_get_int(pT_json_object_stop_sign);

    return 0;
}

/**
 * 0:up 1:down 2:left 3:right 4:forward 5:backward
 */
int update_T_drone_arrn_ultrasound(struct T_drone *pT_drone){
    return 0;
}

int update_T_drone_arrd_yaw_pitch_roll(struct T_drone *pT_drone)
{
	MPU_init();
	while (1)
	{
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

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
		float mx = (float)mrawx*mRes*magCalibration[0] - 406 - 49;  // get actual magnetometer value, this depends on scale being set
		float my = (float)mrawy*mRes*magCalibration[1] - 95 + 43;
		float mz = (float)mrawz*mRes*magCalibration[2] + 370 - 72;
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
#ifdef DEBUG_YAW_PITCH_ROLL
        printf("%.1f, %.1f, %.1f\n",yaw, pitch, roll);
#endif
	}
    return 0;
}

int update_T_drone_arrd_pid_yaw_pitch_roll(struct T_drone *pT_drone){
    pidData_t *pidData_yaw,*pidData_pitch,*pidData_roll;
    pidData_yaw = (pidData_t*) malloc(sizeof(pidData_t));
    pidData_pitch = (pidData_t*) malloc(sizeof(pidData_t));
    pidData_roll = (pidData_t*) malloc(sizeof(pidData_t));
    
	double kp_pitch, ki_pitch, kd_pitch, kp_roll, ki_roll, kd_roll, kp_yaw, ki_yaw, kd_yaw;
	ctrlDir_t controllerDir;
	uint32_t samplePeriodMs;

	// For the nine values, if we can modify them in IOS app, tests can be easier!
	kp_pitch = 0; 
	ki_pitch = 0;
	kd_pitch = 0;   // these three need to be tuning

	kp_roll = 0;
	ki_roll = 0;
	kd_roll = 0;   // these three need to be tuning

	kp_yaw = 0; 
	ki_yaw = 0; 
	kd_yaw = 0;   // these three need to be tuning
    
    samplePeriodMs = 100; //need to be setup
    controllerDir = PID_DIRECT; //Direct control not reverse.

	Pid_Init(pidData_yaw, kp_yaw, ki_yaw, kd_yaw, controllerDir, samplePeriodMs);
    
	Pid_Init(pidData_pitch, kp_pitch, ki_pitch, kd_pitch, controllerDir, samplePeriodMs);
    
	Pid_Init(pidData_roll, kp_roll, ki_roll, kd_roll, controllerDir, samplePeriodMs);
   
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }
        Pid_SetTunings(pidData_yaw, kp_yaw, ki_yaw, kd_yaw);
        Pid_SetTunings(pidData_pitch, kp_pitch, ki_pitch, kd_pitch);
        Pid_SetTunings(pidData_roll, kp_roll, ki_roll, kd_roll);

		//"0" is the setpoint or the destination of the final attitude, representing hovering or suspending. 
		//Replace "0" by HTTP request parameters later.

		// It can be tested after tests for pitch and roll are finished.
		Pid_SetSetPoint(pidData_yaw, 0);						
        Pid_Run(pidData_yaw, pT_drone->arrd_yaw_pitch_roll[0]);
        pT_drone->arrd_pid_yaw_pitch_roll[0] = pidData_yaw->output;

		// For pitch, mainly we can use wires to lock the Y direction. First divide by 2. Adding to pwm1 and pwm2, substracting to pwm3 and pwm4.
		Pid_SetSetPoint(pidData_pitch, 0);
        Pid_Run(pidData_pitch, pT_drone->arrd_yaw_pitch_roll[1]);
        pT_drone->arrd_pid_yaw_pitch_roll[1] = pidData_pitch->output;

        pT_drone->arrd_current_pwm[0] += (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
        pT_drone->arrd_current_pwm[1] += (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);

        pT_drone->arrd_current_pwm[2] -= (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);
        pT_drone->arrd_current_pwm[3] -= (pT_drone->arrd_pid_yaw_pitch_roll[1] / 2);

		// For roll, mainly we can use wires to lock the X direction. First divide by 2. Adding to pwm1 and pwm3, substracting to pwm2 and pwm4.
		Pid_SetSetPoint(pidData_roll, 0); 
        Pid_Run(pidData_roll, pT_drone->arrd_yaw_pitch_roll[2]);
        pT_drone->arrd_pid_yaw_pitch_roll[2] = pidData_roll->output;

        pT_drone->arrd_current_pwm[0] += (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
        pT_drone->arrd_current_pwm[2] += (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);

        pT_drone->arrd_current_pwm[1] -= (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);
        pT_drone->arrd_current_pwm[3] -= (pT_drone->arrd_pid_yaw_pitch_roll[2] / 2);

		usleep(100000); // We need to add some delay to slow down the pid loop. Mainly, 100ms cycle should be good. 
    }
    return 0;
}

int GeneratePwm(struct T_drone *pT_drone){
    mraa_i2c_context pwm12, pwm34;
    pwm12 = mraa_i2c_init(2);
    pwm34 = mraa_i2c_init(6);
    mraa_i2c_address(pwm12, 2); // i2c address 2. It can be arbitrarily defined. For pwm12 output, address = 2
    mraa_i2c_address(pwm34, 3); // i2c address 3. It can be arbitrarily defined. For pwm34 output, address = 3
    double arrd_current_duty[4];
    uint8_t arri_i2c_output[4] = { 0, 0, 0, 0 };
    while(1){

        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

        arrd_current_duty[0] = pT_drone->arrd_current_pwm[0] * 40000;
        arrd_current_duty[1] = pT_drone->arrd_current_pwm[1] * 40000;
        arrd_current_duty[2] = pT_drone->arrd_current_pwm[2] * 40000;
        arrd_current_duty[3] = pT_drone->arrd_current_pwm[3] * 40000;
        
        arri_i2c_output[0] = ((int)arrd_current_duty[0]) / 256;
        arri_i2c_output[1] = ((int)arrd_current_duty[0]) % 256;
        arri_i2c_output[2] = ((int)arrd_current_duty[1]) / 256;
        arri_i2c_output[3] = ((int)arrd_current_duty[1]) % 256;
        mraa_i2c_write(pwm12, arri_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
        
        arri_i2c_output[0] = ((int)arrd_current_duty[2]) / 256;
        arri_i2c_output[1] = ((int)arrd_current_duty[2]) % 256;
        arri_i2c_output[2] = ((int)arrd_current_duty[3]) / 256;
        arri_i2c_output[3] = ((int)arrd_current_duty[3]) % 256;
        mraa_i2c_write(pwm34, arri_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
    }

    /**
     * Stop
     */
    arri_i2c_output[0] = 0;
    arri_i2c_output[1] = 0;
    arri_i2c_output[2] = 0;
    arri_i2c_output[3] = 0;
    mraa_i2c_write(pwm12, arri_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
    mraa_i2c_write(pwm34, arri_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4

    return 0;
}

void ThreadTask_update_T_drone_http_pwm_get(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

        update_T_drone_http_pwm_get(pT_drone);
#ifdef DEBUG_PWM
            printf("pwm1 = %f\n", pT_drone->arrd_current_pwm[0]);
            printf("pwm2 = %f\n", pT_drone->arrd_current_pwm[1]);
            printf("pwm3 = %f\n", pT_drone->arrd_current_pwm[2]);
            printf("pwm4 = %f\n", pT_drone->arrd_current_pwm[3]);
            printf("\n");
#endif
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_http_pwm_post(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

        update_T_drone_http_pwm_post(pT_drone);
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_http(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

        update_T_drone_http(pT_drone);
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_arrd_pid_yaw_pitch_roll(struct T_drone *pT_drone){
    update_T_drone_arrd_pid_yaw_pitch_roll(pT_drone);
}

void ThreadTask_update_T_drone_arrn_ultrasound(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

        update_T_drone_arrn_ultrasound(pT_drone);
    }
}

void ThreadTask_update_T_drone_arrd_yaw_pitch_roll(struct T_drone *pT_drone){
    update_T_drone_arrd_yaw_pitch_roll(pT_drone);
}

void ThreadTask_GeneratePwm(struct T_drone *pT_drone){
    GeneratePwm(pT_drone);
}

void ThreadTask_update_T_drone_http_gps(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }

        update_T_drone_http_gps(pT_drone);
        usleep(50000);
    }
}