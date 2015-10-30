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
#include <mraa/gpio.h>
#include <mraa/pwm.h>

#define PWM_PERIOD_NS 20000000//20ms
//#define PWM_PERIOD_NS 5000000000//5s
#define DEBUG_PWM
//#define DEBUG_GPIO_PWM
#define PWM_DEVIDE_RATIO 100
#define DEBUG_YAW_PITCH_ROLL

//struct T_hash_pwm {
    //const char *pstr_key;          [> key <]
    //double d_pwm;
    //UT_hash_handle hh;         [> makes this structure hashable <]
//};

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

    //GPS info
    double d_current_latitude;
    double d_current_longitude;
    double d_face_direction;
};

//struct T_control {
    //char *sz_mac_address;
    //int n_control_type;
    //int n_auto_control_command;
    //int n_manual_control_command;
    //double arrd_suspend_pwm[4];
//};

//struct T_hash_pwm *g_pT_hash_pwm;
//time_t g_T_timer;
struct T_drone g_T_drone_self;

/**
 * update the pwm value using by test purpose
 */
int update_T_drone_http_pwm(struct T_drone *pT_drone){
    char *sz_url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    /*char *sz_url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";*/
    
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

/** old code with hash
 */
//struct T_hash_pwm* HTTP_get_pT_pwm()
//{
    //char *sz_url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    //[>char *sz_url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";<]
    
    //char *sz_http_response;
    //struct json_object *pT_json_object_whole_response, *ppT_json_object_pwm[4], *pT_json_object_data, *pT_json_object_update_time;
    //int n_json_response;
    //double pd_pwm[4];
    //int n_index=0;

    //sz_http_response = http_get(sz_url_get_pwm);

    //pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    //n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    //n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm1", &ppT_json_object_pwm[0]);
    //n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm2", &ppT_json_object_pwm[1]);
    //n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm3", &ppT_json_object_pwm[2]);
    //n_json_response = json_object_object_get_ex(pT_json_object_data, "pwm4", &ppT_json_object_pwm[3]);
    //n_json_response = json_object_object_get_ex(pT_json_object_data, "update_time", &pT_json_object_update_time);

    //n_index = 0;
    //for(n_index = 0; n_index < 4; n_index++)
    //{
        //pd_pwm[n_index] = json_object_get_double(*(ppT_json_object_pwm+n_index));
    //}

    //const char **kppchIndex, *kstrKeys[] = {"pwm1", "pwm2", "pwm3", "pwm4", NULL};
    //struct T_hash_pwm *pT_pwm_selector, *pT_pwm_all = NULL;

    //n_index = 0;
    //for (kppchIndex = kstrKeys; *kppchIndex != NULL; kppchIndex++) {
        //pT_pwm_selector = (struct T_hash_pwm*)malloc(sizeof(struct T_hash_pwm));
        //if (pT_pwm_selector == NULL) {
            //exit(-1);
        //}
        //pT_pwm_selector->pstr_key = *kppchIndex;
        //pT_pwm_selector->d_pwm = pd_pwm[n_index];
        //n_index++;
        //HASH_ADD_KEYPTR(hh, pT_pwm_all, pT_pwm_selector->pstr_key, strlen(pT_pwm_selector->pstr_key), pT_pwm_selector);
    //}

    //return pT_pwm_all;
//}

//int free_pT_pwm(struct T_hash_pwm *pT_pwm_all){
    //struct T_hash_pwm *pT_pwm_selector, *pT_pwm_tmp;
    //[> free the hash table contents <]
    //HASH_ITER(hh, pT_pwm_all, pT_pwm_selector, pT_pwm_tmp) {
        //HASH_DEL(pT_pwm_all, pT_pwm_selector);
        //free(pT_pwm_selector);
    //}
    //return 0;
//} 

//double get_d_pwm(struct T_hash_pwm *pT_pwm_all, char *sz_pwm_key)
//{
    //struct T_hash_pwm *pT_pwm_selector;
    //double d_pwm;
    //HASH_FIND_STR(pT_pwm_all, sz_pwm_key, pT_pwm_selector);
    //if (pT_pwm_selector != NULL) {
        //d_pwm = pT_pwm_selector->d_pwm;
    //}else{
        //d_pwm = 0;
    //}
    //return d_pwm;
//}

/**
 * update the drone value
 */
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

/**
 * update the drone value for gps
 */
int update_T_drone_http_gps(struct T_drone *pT_drone){
    //How to concat two char* string in C program
    //http://stackoverflow.com/questions/18468229/how-to-concat-two-char-string-in-c-program
    char *sz_url_get_gps = "http://fryer.ee.ucla.edu/rest/api/gps/get/?location_type=0";
    char *sz_url_get_gps_iPhone = "http://fryer.ee.ucla.edu/rest/api/gps/get/?location_type=1";
    char *sz_url_post_gps = "http://fryer.ee.ucla.edu/rest/api/gps/post/?location_type=1";
    
    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *pT_json_object_data, *pT_json_object_update_time, *pT_json_object_face_direction, *pT_json_object_current_latitude, *pT_json_object_current_longitude;
    int n_json_response;

    sz_http_response = http_get(sz_url_get_gps_iPhone);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response, "data", &pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "face_direction", &pT_json_object_face_direction);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "latitude", &pT_json_object_current_latitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data, "longitude", &pT_json_object_current_longitude);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"update_time",&pT_json_object_update_time);

    pT_drone->d_face_direction = json_object_get_int(pT_json_object_face_direction);
    pT_drone->d_current_latitude = json_object_get_int(pT_json_object_current_latitude);
    pT_drone->d_current_longitude = json_object_get_int(pT_json_object_current_longitude);
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
    
	double kp, ki, kd;
	ctrlDir_t controllerDir;
	uint32_t samplePeriodMs;

    kp = 0;     // these three need to be tuning
    ki = 0;
    kd = 0;
    samplePeriodMs = 20; //need to be setup
    controllerDir = PID_DIRECT; //Direct control not reverse.

    Pid_Init(pidData_yaw, kp, ki, kd, controllerDir, samplePeriodMs);
    Pid_SetSetPoint(pidData_yaw, 0);
    Pid_Init(pidData_pitch, kp, ki, kd, controllerDir, samplePeriodMs);
    Pid_SetSetPoint(pidData_pitch, 0);
    Pid_Init(pidData_roll, kp, ki, kd, controllerDir, samplePeriodMs);
    Pid_SetSetPoint(pidData_roll, 0);

    while(1){
        Pid_Run(pidData_yaw, pT_drone->arrd_yaw_pitch_roll[0]);
        pT_drone->arrd_pid_yaw_pitch_roll[0] = pidData_yaw->output;

        Pid_Run(pidData_pitch, pT_drone->arrd_yaw_pitch_roll[1]);
        pT_drone->arrd_pid_yaw_pitch_roll[1] = pidData_pitch->output;

        Pid_Run(pidData_roll, pT_drone->arrd_yaw_pitch_roll[2]);
        pT_drone->arrd_pid_yaw_pitch_roll[2] = pidData_roll->output;
    }
    return 0;
}

int update_T_drone_arrT_timespec(struct T_drone *pT_drone){
    int n_pwm_index = 0;
    for(n_pwm_index = 0; n_pwm_index < 4; n_pwm_index++){
        pT_drone->arrT_timespec_high[n_pwm_index].tv_sec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) / 1000000000;
        pT_drone->arrT_timespec_high[n_pwm_index].tv_nsec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) % 1000000000;
        pT_drone->arrT_timespec_low[n_pwm_index].tv_sec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) / 1000000000;
        pT_drone->arrT_timespec_low[n_pwm_index].tv_nsec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) % 1000000000;
    }
    return 0;
}

/**
 * n_pwm_index = 0,1,2,3
 */
int GeneratePwmFromGpio(struct T_drone *pT_drone, int n_pwm_index, int n_gpio_port){

    mraa_gpio_context gpio;
    gpio = mraa_gpio_init(n_gpio_port);
    mraa_gpio_dir(gpio, MRAA_GPIO_OUT);

    struct timespec T_timespec_high;
    struct timespec T_timespec_low;

    while(1){

        //T_timespec_high.tv_sec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) / 1000000000;
        //T_timespec_high.tv_nsec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) % 1000000000;

        //T_timespec_low.tv_sec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) / 1000000000;
        //T_timespec_low.tv_nsec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) % 1000000000;

#ifdef DEBUG_GPIO_PWM
        printf("pwm%d = %f\n", (n_pwm_index+1), pT_drone->arrd_current_pwm[n_pwm_index]);
#endif

        mraa_gpio_write(gpio, 1);

#ifdef DEBUG_GPIO_PWM
        printf("pwm%d : voltage = 1\n", (n_pwm_index+1));
#endif

        nanosleep(pT_drone->arrT_timespec_high+n_pwm_index, NULL);
        mraa_gpio_write(gpio, 0);

#ifdef DEBUG_GPIO_PWM
        printf("pwm%d : voltage = 0\n", (n_pwm_index+1));
#endif

        nanosleep(pT_drone->arrT_timespec_low+n_pwm_index, NULL);

    }

    return 0;
}

void GeneratePwmFromPwm(struct T_drone *pT_drone){
    mraa_pwm_context pwm;
    pwm = mraa_pwm_init(5);
    mraa_pwm_period_us(pwm, 2000);
    mraa_pwm_enable(pwm, 1);
    struct timespec T_timespec_high;
    struct timespec T_timespec_low;
    T_timespec_high.tv_sec = 0;
    T_timespec_high.tv_nsec = 1 * 1000000;

    T_timespec_low.tv_sec = 0;
    T_timespec_low.tv_nsec = 18.5 * 1000000;

    while(1){
        //update_T_drone_http_pwm(pT_drone);
        mraa_pwm_write(pwm, 0.2);
        //nanosleep(&T_timespec_high, NULL);
        usleep(2000);
        mraa_pwm_write(pwm, 0);
        //mraa_pwm_enable(pwm, 0);
        usleep(3000);
        //nanosleep(&T_timespec_low, NULL);
    }
    //return 0;
}

void ThreadTask_update_T_drone_http_pwm(struct T_drone *pT_drone){
    while(1){
        update_T_drone_http_pwm(pT_drone);
#ifdef DEBUG_PWM
            printf("pwm1 = %f\n", pT_drone->arrd_current_pwm[0]);
            printf("pwm2 = %f\n", pT_drone->arrd_current_pwm[1]);
            printf("pwm3 = %f\n", pT_drone->arrd_current_pwm[2]);
            printf("pwm4 = %f\n", pT_drone->arrd_current_pwm[3]);
            printf("\n");
#endif
        update_T_drone_arrT_timespec(pT_drone);
        usleep(50000);
    }
}

void ThreadTask_update_T_drone_http(struct T_drone *pT_drone){
    while(1){
        update_T_drone_http(pT_drone);
        usleep(50000);
    }
}

//void ThreadTask_HTTP_get_pT_pwm(){
    //while(1){
        //g_pT_hash_pwm = HTTP_get_pT_pwm();
        //usleep(50000);
    //}
//}

//void ThreadTask_get_arrd_pwm(){
    //while(1){
        //g_arrd_current_pwm[0] = get_d_pwm(g_pT_hash_pwm, "pwm1") / 100;
        //g_arrd_current_pwm[1] = get_d_pwm(g_pT_hash_pwm, "pwm2") / 100;
        //g_arrd_current_pwm[2] = get_d_pwm(g_pT_hash_pwm, "pwm3") / 100;
        //g_arrd_current_pwm[3] = get_d_pwm(g_pT_hash_pwm, "pwm4") / 100;
        //printf("pwm1 = %f\n", g_arrd_current_pwm[0]);
        //printf("pwm2 = %f\n", g_arrd_current_pwm[1]);
        //printf("pwm3 = %f\n", g_arrd_current_pwm[2]);
        //printf("pwm4 = %f\n", g_arrd_current_pwm[3]);
        //printf("\n");
        //usleep(50000);
    //}
//}

void ThreadTask_update_T_drone_arrd_pid_yaw_pitch_roll(struct T_drone *pT_drone){
    update_T_drone_arrd_pid_yaw_pitch_roll(pT_drone);
}

void ThreadTask_update_T_drone_arrn_ultrasound(struct T_drone *pT_drone){
    while(1){
        update_T_drone_arrn_ultrasound(pT_drone);
    }
}

void ThreadTask_update_T_drone_arrd_yaw_pitch_roll(struct T_drone *pT_drone){
    update_T_drone_arrd_yaw_pitch_roll(pT_drone);
}

/**
 * n_pwm_index = 0,1,2,3
 */
void ThreadTask_GeneratePwm(int n_pwm_index){
    int n_gpio_port;
    if(n_pwm_index == 0){
        n_gpio_port = 2;
    }else if(n_pwm_index == 1){
        n_gpio_port = 4;
    }else if(n_pwm_index == 2){
        n_gpio_port = 7;
    }else if(n_pwm_index == 3){
        n_gpio_port = 8;
    }
    GeneratePwmFromGpio(&g_T_drone_self, n_pwm_index, n_gpio_port);
}
