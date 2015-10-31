#include "car_control.h"

int main(){
    int i;
    g_T_drone_self->n_stop_sign = 0;
    speed_pwm_in1 = mraa_pwm_init(3);
    speed_pwm_in2 = mraa_pwm_init(5);
    turn_pwm = mraa_pwm_init(6);
	char turn_user_input[MAXBUFSIZ];
    f_speed = 40;
    
    mraa_pwm_period_us(speed_pwm_in1,870); //1150Hz
	mraa_pwm_enable(speed_pwm_in1, 1);
	mraa_pwm_period_us(speed_pwm_in2,870);
	mraa_pwm_enable(speed_pwm_in2, 1);
	
	mraa_pwm_period_ms(turn_pwm,20);
   	mraa_pwm_enable(turn_pwm, 1);
	mraa_pwm_write(turn_pwm, CENTER);
	mraa_pwm_write(speed_pwm_in1, 1.0f);
	mraa_pwm_write(speed_pwm_in2, 1.0f);

    threadpool thpool = thpool_init(10);
    thpool_add_work(thpool, (void*)ThreadTask_update_T_drone_http_gps, (void*)&g_T_drone_self);
    thpool_add_work(thpool, (void*)ThreadTask_GpsNavigationMove, (void*)&g_T_drone_self);
    thpool_wait(thpool);
    thpool_destroy(thpool);

     /*while(1){*/
             /*printf("Turn (L, C, R): ");                                      */
             /*scanf("%s", turn_user_input);               */
                
             /*if (!strcmp(turn_user_input, "L") || !strcmp(turn_user_input, "l"))     */
             /*{*/
                 /*for (i = 0;i<=180;i++){*/
                     /*turn_left();*/
                 /*}*/
             /*}else if (!strcmp(turn_user_input, "C") || !strcmp(turn_user_input, "c"))*/
             /*{*/
                 /*for (i = 0;i<=50;i++){*/
                     /*move_forward(0.0);*/
                 /*}*/
             /*}               */
             /*else if (!strcmp(turn_user_input, "R") || !strcmp(turn_user_input, "r")){*/
                 /*for (i = 0;i<=180;i++){*/
                     /*turn_right();*/
                 /*} */
             /*}*/
             /*else if(!strcmp(turn_user_input, "S") || !strcmp(turn_user_input, "s"))*/
             /*{*/
                  /*f_turn = CENTER;*/
                  /*mraa_pwm_write(turn_pwm, f_turn);*/
                  /*usleep(10000);*/
                  /*speed_control(speed_pwm_in1, speed_pwm_in2, 0);*/
             /*}*/
             /*else {                                 */
                     /*printf("Wrong turn type!\n");*/
              /*return 1;*/
         /*}*/
        
         /*sleep(1);*/
         /*speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);*/
     /*} */

    f_turn = CENTER;
    mraa_pwm_write(turn_pwm, f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, 0);
    printf("stop\n");
    return 0;

}
