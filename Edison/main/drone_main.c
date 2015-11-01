#include "include/drone_include.h"

int main()
{
    mraa_init(); // can we put it in the beginning. Avoid repeating definition.
    g_T_drone_self.sz_mac_address = "fc:c2:de:3d:7f:af";
    threadpool thpool = thpool_init(10);

    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_pwm_post, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_arrd_yaw_pitch_roll, (void*)&g_T_drone_self);

    thpool_wait(thpool);
    thpool_destroy(thpool);

    return 0;
}
