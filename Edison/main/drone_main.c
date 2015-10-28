#include "include/drone_include.h"

int main()
{
    /*threadpool thpool = thpool_init(10);*/
    /*thpool_add_work(thpool, (void*)ThreadTask_HTTP_get_pT_pwm, NULL);*/
    /*thpool_add_work(thpool, (void*)ThreadTask_get_arrd_pwm, NULL);*/
    /*thpool_wait(thpool);*/
    /*thpool_destroy(thpool);*/
    /*free_pT_pwm(g_pT_hash_pwm);*/
    while(1){
        HTTP_update_T_drone_pwm(&g_T_drone_my);
        printf("pwm1 = %f\n", g_T_drone_my.arrd_current_pwm[0]);
        printf("pwm2 = %f\n", g_T_drone_my.arrd_current_pwm[1]);
        printf("pwm3 = %f\n", g_T_drone_my.arrd_current_pwm[2]);
        printf("pwm4 = %f\n", g_T_drone_my.arrd_current_pwm[3]);
        usleep(50000);
    }
    return 0;
}
