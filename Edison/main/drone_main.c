#include "include/drone_include.h"

int main()
{
    mraa_init(); // can we put it in the beginning. Avoid repeating definition.
    initialize_struct_T_drone(&g_T_drone_self);

    threadpool thpool = thpool_init(10);

    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http, (void*)&g_T_drone_self);
    // thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)&g_T_drone_self);
    // thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_pid_tuning_get, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_arrd_yaw_pitch_roll, (void*)&g_T_drone_self);
    // thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_pwm_post, (void*)&g_T_drone_self);
    // thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_arrd_pid, (void*)&g_T_drone_self);

    thpool_wait(thpool);
    thpool_destroy(thpool);

    initialize_struct_T_drone(&g_T_drone_self);
    printf("The drone program has been stopped.\n");
    return 0;
}