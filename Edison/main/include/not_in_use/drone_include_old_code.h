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