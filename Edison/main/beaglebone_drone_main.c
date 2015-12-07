#include "include/beaglebone_drone_include.h"

int main()
{
    /*mraa_init(); // can we put it in the beginning. Avoid repeating definition.*/
    /**
     * initialize_struct_T_drone
     */
    initialize_struct_T_drone(&g_T_drone_self);
    /**
     * threadpool
     */
    threadpool thpool = thpool_init(10);
    /**
     * add work to thread
     */
    thpool_add_work(thpool, (void*)ThreadTask_GeneratePwm, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_arrd_yaw_pitch_roll, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_uart_message, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_arrd_pid, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_gps, (void*)&g_T_drone_self);
    /*thpool_add_work(thpool, (void*)ThreadTask_CalibrateEsc, (void*)&g_T_drone_self);*/
    /**
     * pwm only for testing
     */
    // thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_pwm_get, (void*)&g_T_drone_self);
    /**
     * wait threads to end
     */
    thpool_wait(thpool);
    thpool_destroy(thpool);
    /**
     * set all variables in the global structure to be 0
     */
    initialize_struct_T_drone(&g_T_drone_self);
    printf("The program has been stopped.\n");
    return 0;
}
