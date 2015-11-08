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
	mraa_pwm_write(speed_pwm_in1, 0.0f);
	mraa_pwm_write(speed_pwm_in2, 0.0f);

    /*ThreadTask_GpsNavigationMove(&g_T_drone_self);*/

    threadpool thpool = thpool_init(10);
     /*thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_gps, (void*)&g_T_drone_self);*/
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http, (void*)&g_T_drone_self);
    // thpool_add_work(thpool, (void*)ThreadTask_sonicTurn_pwm, (void*)&g_T_drone_self);
     /*thpool_add_work(thpool, (void*)ThreadTask_GpsNavigationMove, (void*)&g_T_drone_self);*/
    /*thpool_add_work(thpool, (void*)ThreadTask_Ultrasonic_read_left, (void*)&g_T_drone_self);*/
    // thpool_add_work(thpool, (void*)ThreadTask_Ultrasonic_read_right, (void*)&g_T_drone_self);
    /*thpool_add_work(thpool, (void*)ThreadTask_Ultrasonic_read_center, (void*)&g_T_drone_self);*/
    // thpool_add_work(thpool, (void*)ThreadTask_manual_control, (void*)&g_T_drone_self);
    thpool_wait(thpool);
    thpool_destroy(thpool);


     float speed, turn;
	char speed_user_input[MAXBUFSIZ];
	char turn_user_input[MAXBUFSIZ];
	int i, case_num, averageTimes = 3, speed_flag = 1;
	double distance_l, distance_c, distance_r, distance_slight_l, distance_slight_r;
	mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r, trig_up_l, echo_up_l, trig_up_r, echo_up_r;
	mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;

    trig_l = mraa_gpio_init(10);                                                     
	echo_l = mraa_gpio_init(11);
	trig_c = mraa_gpio_init(7);
	echo_c = mraa_gpio_init(8);
	trig_r = mraa_gpio_init(12);
	echo_r = mraa_gpio_init(13);
	
	if  (trig_c == NULL || echo_c == NULL || trig_l == NULL || echo_l == NULL || trig_r == NULL || echo_r == NULL) {
		fprintf(stderr, "Failed to initialized.\n");
	}

	mraa_gpio_dir(trig_l, MRAA_GPIO_OUT);                                   
	mraa_gpio_dir(echo_l, MRAA_GPIO_IN);
	mraa_gpio_dir(trig_c, MRAA_GPIO_OUT);
	mraa_gpio_dir(echo_c, MRAA_GPIO_IN);
	mraa_gpio_dir(trig_r, MRAA_GPIO_OUT);
	mraa_gpio_dir(echo_r, MRAA_GPIO_IN);
    
    n_ultrasonic_degree = 0;
    
	//At the start, the car will not move and the steering will be at center.   
    while(1){

        if (pT_drone->nflag_stop_all != 0)
        {
            break;
        }else if(pT_drone->n_control_type != 1){
            continue;
        }
        
        if(n_ultrasonic_degree == 0){
		    distance_c = get_distance(trig_c, echo_c);

        }else if(n_ultrasonic_degree == -1){
            distance_slight_l = get_distance(trig_c, echo_c);
        }else if(n_ultrasonic_degree == 1){ 
		    distance_slight_r = get_distance(trig_c, echo_c);
        }
		distance_l = get_distance(trig_l, echo_l);	
		distance_r = get_distance(trig_r, echo_r);

        printf("lefet distance = %f \n", distance_l);
#ifdef PRINT_DEBUG_THREAD
      printf("ThreadTask_GpsNavigationMove\n");
#endif
              
        if(distance_l <= 60 && distance_r <= 60 && distance_c <=60){
            obstacle_case1(&g_T_drone_self, distance_l, distance_slight_l, distance_c, distance_slight_r, distance_r);
        }else if(distance_l > 60 && distance_r > 60){
            if(distance_c <= 60 || distance_slight_l <= 60 || distance_slight_r <= 60){
                obstacle_case2(&g_T_drone_self, distance_l, distance_slight_l, distance_c, distance_slight_r, distance_r);
            }
        }else if(distance_l <= 60 || distance_r <= 60 && distance_c <= 60){
            obstacle_case3(&g_T_drone_self, distance_l, distance_slight_l, distance_c, distance_slight_r, distance_r); 
        }else{
             GpsNavigationMove(pT_drone);
        }
        usleep(50000);
    }
}
 
    g_f_turn = CENTER;
    mraa_pwm_write(turn_pwm, g_f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, 0);
    printf("stop\n");
    return 0;

}
