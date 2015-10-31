#include "include/drone_include.h"

int main()
{
    mraa_init(); // can we put it in the beginning. Avoid repeating definition.
    g_T_drone_self.sz_mac_address = "fc:c2:de:3d:7f:af";
    threadpool thpool = thpool_init(10);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_pwm, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_arrd_yaw_pitch_roll, (void*)&g_T_drone_self);
    /*thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)0);*/
    /*thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)1);*/
    /*thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)2);*/
    /*thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)3);*/
    thpool_wait(thpool);
    thpool_destroy(thpool);
    /*free_pT_pwm(g_pT_hash_pwm);*/
    /*while(1){*/
        /*HTTP_update_T_drone_pwm(&g_T_drone_my);*/
        /*printf("pwm1 = %f\n", g_T_drone_my.arrd_current_pwm[0]);*/
        /*printf("pwm2 = %f\n", g_T_drone_my.arrd_current_pwm[1]);*/
        /*printf("pwm3 = %f\n", g_T_drone_my.arrd_current_pwm[2]);*/
        /*printf("pwm4 = %f\n", g_T_drone_my.arrd_current_pwm[3]);*/
        /*usleep(50000);*/
    /*}*/
    return 0;
}
