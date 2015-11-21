//#include <mraa/gpio.h>
//#include <mraa/pwm.h>

//struct T_hash_pwm {
    //const char *pstr_key;          [> key <]
    //double d_pwm;
    //UT_hash_handle hh;         [> makes this structure hashable <]
//};

//struct T_control {
    //char *sz_mac_address;
    //int n_control_type;
    //int n_auto_control_command;
    //int n_manual_control_command;
    //double arrd_suspend_pwm[4];
//};

    // struct timespec arrT_timespec_high[4];
    // struct timespec arrT_timespec_low[4];

//struct T_hash_pwm *g_pT_hash_pwm;
//time_t g_T_timer;

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

// int update_T_drone_arrT_timespec(struct T_drone *pT_drone){
//     int n_pwm_index = 0;
//     for(n_pwm_index = 0; n_pwm_index < 4; n_pwm_index++){
//         pT_drone->arrT_timespec_high[n_pwm_index].tv_sec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) / 1000000000;
//         pT_drone->arrT_timespec_high[n_pwm_index].tv_nsec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) % 1000000000;
//         pT_drone->arrT_timespec_low[n_pwm_index].tv_sec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) / 1000000000;
//         pT_drone->arrT_timespec_low[n_pwm_index].tv_nsec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) % 1000000000;
//     }
//     return 0;
// }

/**
 * n_pwm_index = 0,1,2,3
 */
// int GeneratePwmFromGpio(struct T_drone *pT_drone, int n_pwm_index, int n_gpio_port){

//     mraa_gpio_context gpio;
//     gpio = mraa_gpio_init(n_gpio_port);
//     mraa_gpio_dir(gpio, MRAA_GPIO_OUT);

//     struct timespec T_timespec_high;
//     struct timespec T_timespec_low;

//     while(1){

//         if (pT_drone->n_stop_sign == 1)
//         {
//             break;
//         }

//         //T_timespec_high.tv_sec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) / 1000000000;
//         //T_timespec_high.tv_nsec = ((int)round(PWM_PERIOD_NS * pT_drone->arrd_current_pwm[n_pwm_index])) % 1000000000;

//         //T_timespec_low.tv_sec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) / 1000000000;
//         //T_timespec_low.tv_nsec = ((int)round(PWM_PERIOD_NS * ( 1 - pT_drone->arrd_current_pwm[n_pwm_index] ))) % 1000000000;

// #ifdef DEBUG_GPIO_PWM
//         printf("pwm%d = %f\n", (n_pwm_index+1), pT_drone->arrd_current_pwm[n_pwm_index]);
// #endif

//         mraa_gpio_write(gpio, 1);

// #ifdef DEBUG_GPIO_PWM
//         printf("pwm%d : voltage = 1\n", (n_pwm_index+1));
// #endif

//         nanosleep(pT_drone->arrT_timespec_high+n_pwm_index, NULL);
//         mraa_gpio_write(gpio, 0);

// #ifdef DEBUG_GPIO_PWM
//         printf("pwm%d : voltage = 0\n", (n_pwm_index+1));
// #endif

//         nanosleep(pT_drone->arrT_timespec_low+n_pwm_index, NULL);

//     }

//     return 0;
// }


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


/**
 * n_pwm_index = 0,1,2,3
 */
// void ThreadTask_GeneratePwmFromGpio(int n_pwm_index){
//     int n_gpio_port;
//     if(n_pwm_index == 0){
//         n_gpio_port = 2;
//     }else if(n_pwm_index == 1){
//         n_gpio_port = 4;
//     }else if(n_pwm_index == 2){
//         n_gpio_port = 7;
//     }else if(n_pwm_index == 3){
//         n_gpio_port = 8;
//     }
//     GeneratePwmFromGpio(&g_T_drone_self, n_pwm_index, n_gpio_port);
// }

        // update_T_drone_arrT_timespec(pT_drone);

    /*free_pT_pwm(g_pT_hash_pwm);*/
    /*while(1){*/
        /*HTTP_update_T_drone_pwm(&g_T_drone_my);*/
        /*printf("pwm1 = %f\n", g_T_drone_my.arrd_current_pwm[0]);*/
        /*printf("pwm2 = %f\n", g_T_drone_my.arrd_current_pwm[1]);*/
        /*printf("pwm3 = %f\n", g_T_drone_my.arrd_current_pwm[2]);*/
        /*printf("pwm4 = %f\n", g_T_drone_my.arrd_current_pwm[3]);*/
        /*usleep(50000);*/
    /*}*/

/*
int update_T_drone_arrd_yaw_pitch_roll_old_i2c(struct T_drone *pT_drone)
{
#ifdef TIMER_YAW_PITCH_ROLL
timer_start(&g_timer);
#endif
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
#ifdef TIMER_YAW_PITCH_ROLL
        g_last_time_us = timer_delta_us(&g_timer);
        timer_unpause(&g_timer);
#endif
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
        float mx = (float)mrawx*mRes*magCalibration[0] - 406 - 49 - 150 + 72;  // get actual magnetometer value, this depends on scale being set
        float my = (float)mrawy*mRes*magCalibration[1] - 95 + 43 + 15 - 178;
        float mz = (float)mrawz*mRes*magCalibration[2] + 370 - 72 + 403 - 447;
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
        if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1){
            n_index_yaw_pitch_roll++;
            n_index_yaw_pitch_roll = n_index_yaw_pitch_roll%10;
            if(n_index_yaw_pitch_roll == 0){
                printf("yaw = %.1f\tpitch = %.1f\troll = %.1f\n",yaw, pitch, roll);
            }
        }
#ifdef TIMER_YAW_PITCH_ROLL
        timer_pause(&g_timer);
        printf("Delta (us): %ld\n", timer_delta_us(&g_timer) - g_last_time_us);
#endif
#endif
    }
    mraa_i2c_stop(mpu);
    // mraa_gpio_close(gpio_vcc);
    return 0;
}
*/
//int GeneratePwm_old_i2c(struct T_drone *pT_drone){
    //usleep(1000000);
    //mraa_i2c_context pwm12, pwm34;
    //pwm12 = mraa_i2c_init(2);
    //pwm34 = mraa_i2c_init(6);
    //mraa_i2c_address(pwm12, 2); // i2c address 2. It can be arbitrarily defined. For pwm12 output, address = 2
    //mraa_i2c_address(pwm34, 3); // i2c address 3. It can be arbitrarily defined. For pwm34 output, address = 3
    //double arrd_current_duty[4];
    //uint8_t arrun_i2c_output[4] = { 0, 0, 0, 0 };
    //while(1){
//#ifdef PRINT_DEBUG_THREAD
        //printf("ThreadTask_GeneratePwm\n");
//#endif
//#ifdef PRINT_DEBUG_PWM
        //printf("pwm1 = %f\t", pT_drone->arrd_current_pwm[0]);
        //printf("pwm2 = %f\t", pT_drone->arrd_current_pwm[1]);
        //printf("pwm3 = %f\t", pT_drone->arrd_current_pwm[2]);
        //printf("pwm4 = %f\n", pT_drone->arrd_current_pwm[3]);
//#endif
        //if (pT_drone->nflag_stop_all != 0)
        //{
            /**
             * Reset PWM to 0
             */
            //pT_drone->arrd_current_pwm[0] = 0;
            //pT_drone->arrd_current_pwm[1] = 0;
            //pT_drone->arrd_current_pwm[2] = 0;
            //pT_drone->arrd_current_pwm[3] = 0;
            //arrun_i2c_output[0] = 0;
            //arrun_i2c_output[1] = 0;
            //arrun_i2c_output[2] = 0;
            //arrun_i2c_output[3] = 0;
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1)
        //{
            /**
             * Reset PWM to 0
             */
            //pT_drone->arrd_current_pwm[0] = 0;
            //pT_drone->arrd_current_pwm[1] = 0;
            //pT_drone->arrd_current_pwm[2] = 0;
            //pT_drone->arrd_current_pwm[3] = 0;
            //arrun_i2c_output[0] = 0;
            //arrun_i2c_output[1] = 0;
            //arrun_i2c_output[2] = 0;
            //arrun_i2c_output[3] = 0;
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
            //continue;
        //}
        /**
         * set pwm wave
         */
        //arrd_current_duty[0] = pT_drone->arrd_current_pwm[0] * 40000;
        //arrd_current_duty[1] = pT_drone->arrd_current_pwm[1] * 40000;
        //arrd_current_duty[2] = pT_drone->arrd_current_pwm[2] * 40000;
        //arrd_current_duty[3] = pT_drone->arrd_current_pwm[3] * 40000;
        /**
         * set pwm1 and pwm2
         */
        //arrun_i2c_output[0] = ((int)arrd_current_duty[0]) / 256;
        //arrun_i2c_output[1] = ((int)arrd_current_duty[0]) % 256;
        //arrun_i2c_output[2] = ((int)arrd_current_duty[1]) / 256;
        //arrun_i2c_output[3] = ((int)arrd_current_duty[1]) % 256;
        //if (pT_drone->nflag_stop_all != 0)
        //{
            /**
             * Reset PWM to 0
             */
            //pT_drone->arrd_current_pwm[0] = 0;
            //pT_drone->arrd_current_pwm[1] = 0;
            //pT_drone->arrd_current_pwm[2] = 0;
            //pT_drone->arrd_current_pwm[3] = 0;
            //arrun_i2c_output[0] = 0;
            //arrun_i2c_output[1] = 0;
            //arrun_i2c_output[2] = 0;
            //arrun_i2c_output[3] = 0;
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1)
        //{
            /**
             * Reset PWM to 0
             */
            //pT_drone->arrd_current_pwm[0] = 0;
            //pT_drone->arrd_current_pwm[1] = 0;
            //pT_drone->arrd_current_pwm[2] = 0;
            //pT_drone->arrd_current_pwm[3] = 0;
            //arrun_i2c_output[0] = 0;
            //arrun_i2c_output[1] = 0;
            //arrun_i2c_output[2] = 0;
            //arrun_i2c_output[3] = 0;
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
            //continue;
        //}else{
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
        }
        /**
         * set pwm3 and pwm4
         */
        //arrun_i2c_output[0] = ((int)arrd_current_duty[2]) / 256;
        //arrun_i2c_output[1] = ((int)arrd_current_duty[2]) % 256;
        //arrun_i2c_output[2] = ((int)arrd_current_duty[3]) / 256;
        //arrun_i2c_output[3] = ((int)arrd_current_duty[3]) % 256;
        //if (pT_drone->nflag_stop_all != 0)
        //{
            /**
             * Reset PWM to 0
             */
            //pT_drone->arrd_current_pwm[0] = 0;
            //pT_drone->arrd_current_pwm[1] = 0;
            //pT_drone->arrd_current_pwm[2] = 0;
            //pT_drone->arrd_current_pwm[3] = 0;
            //arrun_i2c_output[0] = 0;
            //arrun_i2c_output[1] = 0;
            //arrun_i2c_output[2] = 0;
            //arrun_i2c_output[3] = 0;
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
            //break;
        //}else if (pT_drone->nflag_enable_pwm_pid_ultrasound != 1)
        //{
            /**
             * Reset PWM to 0
             */
            //pT_drone->arrd_current_pwm[0] = 0;
            //pT_drone->arrd_current_pwm[1] = 0;
            //pT_drone->arrd_current_pwm[2] = 0;
            //pT_drone->arrd_current_pwm[3] = 0;
            //arrun_i2c_output[0] = 0;
            //arrun_i2c_output[1] = 0;
            //arrun_i2c_output[2] = 0;
            //arrun_i2c_output[3] = 0;
            //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
            //continue;
        //}else{
            //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
        //}
    //}
    /**
     * Reset PWM to 0
     */
    //pT_drone->arrd_current_pwm[0] = 0;
    //pT_drone->arrd_current_pwm[1] = 0;
    //pT_drone->arrd_current_pwm[2] = 0;
    //pT_drone->arrd_current_pwm[3] = 0;
    //arrun_i2c_output[0] = 0;
    //arrun_i2c_output[1] = 0;
    //arrun_i2c_output[2] = 0;
    //arrun_i2c_output[3] = 0;
    //mraa_i2c_write(pwm12, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2
    //mraa_i2c_write(pwm34, arrun_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
    //mraa_i2c_stop(pwm12);
    //mraa_i2c_stop(pwm34);
    //return 0;
//}
