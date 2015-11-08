#include "car_control.h"

int main(){
	trig_l = mraa_gpio_init(10);                                                     
	echo_l = mraa_gpio_init(11);
	trig_c = mraa_gpio_init(7);
	echo_c = mraa_gpio_init(8);
	trig_r = mraa_gpio_init(12);
	echo_r = mraa_gpio_init(13);
	speed_pwm_in1 = mraa_pwm_init(3);
	speed_pwm_in2 = mraa_pwm_init(5);
	turn_pwm = mraa_pwm_init(6);

    sonicTurn_pwm = mraa_pwm_init(9);

    initialize_struct_T_drone(&g_T_drone_self);
    

	if(trig_c == NULL || echo_c == NULL || trig_l == NULL || echo_l == NULL || trig_r == NULL || echo_r == NULL || speed_pwm_in1 == NULL || speed_pwm_in2 == NULL || turn_pwm == NULL) {
		fprintf(stderr, "Failed to initialized.\n");
		return 1;
	}

	mraa_gpio_dir(trig_l, MRAA_GPIO_OUT);                                   
	mraa_gpio_dir(echo_l, MRAA_GPIO_IN);
	mraa_gpio_dir(trig_c, MRAA_GPIO_OUT);
	mraa_gpio_dir(echo_c, MRAA_GPIO_IN);
	mraa_gpio_dir(trig_r, MRAA_GPIO_OUT);
	mraa_gpio_dir(echo_r, MRAA_GPIO_IN);

	//Lego(R) M-Motor configuration
	mraa_pwm_period_us(speed_pwm_in1,870); //1150Hz
	mraa_pwm_enable(speed_pwm_in1, 1);
	mraa_pwm_period_us(speed_pwm_in2,870);
	mraa_pwm_enable(speed_pwm_in2, 1);

	//Servo configuration
	mraa_pwm_period_ms(turn_pwm, 20);
   	mraa_pwm_enable(turn_pwm, 1);
    
    mraa_pwm_enable(sonicTurn_pwm,1);

    mraa_pwm_write(sonicTurn_pwm, CENTER);
	//At the start, the car will not move and the steering will be at center.
	speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
	mraa_pwm_write(turn_pwm, 0.067f);

	sleep(1);
	
	speed_control(speed_pwm_in1, speed_pwm_in2, 100.0f);


    threadpool thpool = thpool_init(10);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_gps, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_sonicTurn_pwm, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_GpsNavigationMove, (void*)&g_T_drone_self);
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
