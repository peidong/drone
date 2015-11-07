#include "car_control.h"

int main(){
    speed_pwm_in1 = mraa_pwm_init(3);
    speed_pwm_in2 = mraa_pwm_init(5);
    turn_pwm = mraa_pwm_init(6);
    sonicTurn_pwm = mraa_pwm_init(9);
    initialize_struct_T_drone(&g_T_drone_self);
    
    char turn_user_input[MAXBUFSIZ];
    g_f_speed = 40;
    
    mraa_pwm_period_us(speed_pwm_in1,870); //1150Hz
	mraa_pwm_enable(speed_pwm_in1, 1);
	mraa_pwm_period_us(speed_pwm_in2,870);
	mraa_pwm_enable(speed_pwm_in2, 1);	
	mraa_pwm_period_ms(turn_pwm,20);
   	mraa_pwm_enable(turn_pwm, 1);
    mraa_pwm_period_ms(sonicTurn_pwm,20);
    mraa_pwm_enable(sonicTurn_pwm,1);

    mraa_pwm_write(sonicTurn_pwm, CENTER);
	mraa_pwm_write(turn_pwm, CENTER);
	// mraa_pwm_write(speed_pwm_in1, 1.0f);
	// mraa_pwm_write(speed_pwm_in2, 1.0f);

    threadpool thpool = thpool_init(10);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_gps, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_sonicTurn_pwm, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_GpsNavigationMove, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_Ultrasonic_read, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_manual_control, (void*)&g_T_drone_self);
    thpool_wait(thpool);
    thpool_destroy(thpool);

 
    g_f_turn = CENTER;
    mraa_pwm_write(turn_pwm, g_f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, 0);
    printf("stop\n");
    return 0;

}
