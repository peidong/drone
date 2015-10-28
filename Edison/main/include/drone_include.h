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

#define PWM_PERIOD_NS 20000000

struct T_hash_pwm {
    const char *pstr_key;          /* key */
    double d_pwm;
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct T_control {
    char *sz_mac_address;
    int n_control_type;
    int n_auto_control_command;
    int n_manual_control_command;
    double arrd_suspend_pwm[4];
};

struct T_hash_pwm *g_pT_hash_pwm;
double g_arrd_current_pwm[4];
struct T_control *g_pT_control;
double g_arrd_control[4];
int g_arrn_ultrasound[6];/*0:up 1:down 2:left 3:right 4:forward 5:backward*/
double g_arrd_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
double g_arrd_Pid_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/
//time_t g_T_timer;
struct T_control g_T_my_control;


struct T_hash_pwm* HTTP_get_pT_pwm()
{
    char *sz_url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    /*char *sz_url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";*/
    
    char *sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_pwm[4], *pT_json_object_data, *pT_json_object_update_time;
    int n_json_response;
    double pd_pwm[4];
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
        pd_pwm[n_index] = json_object_get_double(*(ppT_json_object_pwm+n_index));
    }

    const char **kppchIndex, *kstrKeys[] = {"pwm1", "pwm2", "pwm3", "pwm4", NULL};
    struct T_hash_pwm *pT_pwm_selector, *pT_pwm_all = NULL;

    n_index = 0;
    for (kppchIndex = kstrKeys; *kppchIndex != NULL; kppchIndex++) {
        pT_pwm_selector = (struct T_hash_pwm*)malloc(sizeof(struct T_hash_pwm));
        if (pT_pwm_selector == NULL) {
            exit(-1);
        }
        pT_pwm_selector->pstr_key = *kppchIndex;
        pT_pwm_selector->d_pwm = pd_pwm[n_index];
        n_index++;
        HASH_ADD_KEYPTR(hh, pT_pwm_all, pT_pwm_selector->pstr_key, strlen(pT_pwm_selector->pstr_key), pT_pwm_selector);
    }

    return pT_pwm_all;
}

int free_pT_pwm(struct T_hash_pwm *pT_pwm_all){
    struct T_hash_pwm *pT_pwm_selector, *pT_pwm_tmp;
    /* free the hash table contents */
    HASH_ITER(hh, pT_pwm_all, pT_pwm_selector, pT_pwm_tmp) {
        HASH_DEL(pT_pwm_all, pT_pwm_selector);
        free(pT_pwm_selector);
    }
    return 0;
} 

double get_d_pwm(struct T_hash_pwm *pT_pwm_all, char *sz_pwm_key)
{
    struct T_hash_pwm *pT_pwm_selector;
    double d_pwm;
    HASH_FIND_STR(pT_pwm_all, sz_pwm_key, pT_pwm_selector);
    if (pT_pwm_selector != NULL) {
        d_pwm = pT_pwm_selector->d_pwm;
    }else{
        d_pwm = 0;
    }
    return d_pwm;
}

int HTTP_update_T_control(struct T_control *pT_control){
    //How to concat two char* string in C program
    //http://stackoverflow.com/questions/18468229/how-to-concat-two-char-string-in-c-program
    g_T_my_control.sz_mac_address = "fc:c2:de:3d:7f:af";
    char *sz_url_get_control_part1 = "http://fryer.ee.ucla.edu/rest/api/control/get/?mac_address=";
    char *sz_url_get_control = (char*) malloc(1 + strlen(sz_url_get_control_part1) + strlen(g_T_my_control.sz_mac_address));
    strcpy(sz_url_get_control, sz_url_get_control_part1);
    strcat(sz_url_get_control, g_T_my_control.sz_mac_address);
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

    pT_control->n_control_type = json_object_get_int(pT_json_object_control_type);
    pT_control->n_auto_control_command = json_object_get_int(pT_json_object_auto_control_command);
    pT_control->n_manual_control_command = json_object_get_int(pT_json_object_manual_control_command);
    for(n_index = 0; n_index < 4; n_index++)
    {
        pT_control->arrd_suspend_pwm[n_index] = json_object_get_double(*(ppT_json_object_suspend_pwm + n_index));
    }
    return 0;
}

/**
 * 0:up 1:down 2:left 3:right 4:forward 5:backward
 */
void update_g_arrn_ultrasound(){
}

void update_g_arrd_yaw_pitch_roll()
{
	MPU_init();
	while (1)
	{
		uint8_t Buf[14];
		mraa_i2c_read_bytes_data(mpu, 59, Buf, 14);
		// Accelerometer
		int16_t arawx = -(Buf[0] << 8 | Buf[1]) - 170;
		int16_t arawy = -(Buf[2] << 8 | Buf[3]) + 600;
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

		//		printf("%6d,%6d,%6d\n",arawx, arawy, arawz);
		//		printf("%6d,%6d,%6d\n",grawx, grawy, grawz);    

		float ax = (float)arawx*aRes;
		float ay = (float)arawy*aRes;
		float az = (float)arawz*aRes;
		float gx = (float)grawx*gRes;
		float gy = (float)grawy*gRes;
		float gz = (float)grawz*gRes;
		float mx = (float)mrawx*mRes*magCalibration[0] - 406;  // get actual magnetometer value, this depends on scale being set
		float my = (float)mrawy*mRes*magCalibration[1] - 95;
		float mz = (float)mrawz*mRes*magCalibration[2] + 370;
		//    printf("%.1f,%.1f,%.1f\n",mx,my,mz);

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

		g_arrd_yaw_pitch_roll[0] = yaw;
		g_arrd_yaw_pitch_roll[1] = pitch;
		g_arrd_yaw_pitch_roll[2] = roll;
		//    printf("%.1f, %.1f, %.1f\n",yaw, pitch, roll);
	}
}

int GeneratePwm(double d_pwm_duty_cycle){

    struct timespec T_timespec_high;
    struct timespec T_timespec_low;

    T_timespec_high.tv_sec = ((int)round(PWM_PERIOD_NS * d_pwm_duty_cycle)) / 1000000000;
    T_timespec_high.tv_nsec = ((int)round(PWM_PERIOD_NS * d_pwm_duty_cycle)) % 1000000000;

    T_timespec_low.tv_sec = ((int)round(PWM_PERIOD_NS * ( 1 - d_pwm_duty_cycle ))) / 1000000000;
    T_timespec_low.tv_nsec = ((int)round(PWM_PERIOD_NS * ( 1 - d_pwm_duty_cycle ))) % 1000000000;

    return 0;
}

void ThreadTask_HTTP_get_pT_pwm(){
    while(1){
        g_pT_hash_pwm = HTTP_get_pT_pwm();
        usleep(50000);
    }
}

void ThreadTask_get_arrd_pwm(){
    while(1){
        g_arrd_current_pwm[0] = get_d_pwm(g_pT_hash_pwm, "pwm1") / 100;
        g_arrd_current_pwm[1] = get_d_pwm(g_pT_hash_pwm, "pwm2") / 100;
        g_arrd_current_pwm[2] = get_d_pwm(g_pT_hash_pwm, "pwm3") / 100;
        g_arrd_current_pwm[3] = get_d_pwm(g_pT_hash_pwm, "pwm4") / 100;
        printf("pwm1 = %f\n", g_arrd_current_pwm[0]);
        printf("pwm2 = %f\n", g_arrd_current_pwm[1]);
        printf("pwm3 = %f\n", g_arrd_current_pwm[2]);
        printf("pwm4 = %f\n", g_arrd_current_pwm[3]);
        printf("\n");
        usleep(50000);
    }
}

void ThreadTask_Pid(){

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
        Pid_Run(pidData_yaw, g_arrd_yaw_pitch_roll[0]);
        g_arrd_Pid_yaw_pitch_roll[0] = pidData_yaw->output;

        Pid_Run(pidData_pitch, g_arrd_yaw_pitch_roll[1]);
        g_arrd_Pid_yaw_pitch_roll[1] = pidData_pitch->output;

        Pid_Run(pidData_roll, g_arrd_yaw_pitch_roll[2]);
        g_arrd_Pid_yaw_pitch_roll[2] = pidData_roll->output;
    }


}

void ThreadTask_update_ultrasound(){
    while(1){
        update_g_arrn_ultrasound();
    }
}

void ThreadTask_update_yaw_pitch_roll(){
	update_g_arrd_yaw_pitch_roll();
}

void ThreadTask_HTTP_get_control(){
}

//int main()
//{
    //threadpool thpool = thpool_init(10);
    //thpool_add_work(thpool, (void*)ThreadTask_HTTP_get_pT_pwm, NULL);
    //thpool_add_work(thpool, (void*)ThreadTask_get_arrd_pwm, NULL);
    //thpool_wait(thpool);
    //thpool_destroy(thpool);
    //free_pT_pwm(g_pT_hash_pwm);
    //return 0;
//}
