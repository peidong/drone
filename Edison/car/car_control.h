#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.048f
#define RIGHTMAX 0.088f

const double EARTH_RADIUS = 6378137;
long distance_l, distance_c, distance_r, distance_slight_l, distance_slight_r;                                                                                   
float f_speed, f_turn;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;

/*
 *  This is for ultrasonic sensor read
 *
 * */
sig_atomic_t volatile isrunning =1;                                              
 
void do_when_interrupted(int sig) {
        if (sig == SIGINT)
            isrunning = 0;
}

long get_distance(mraa_gpio_context trigger, mraa_gpio_context echo)
{ 
    long distance, time1, time2;
    mraa_gpio_write(trigger, 0);
    usleep(10);
    mraa_gpio_write(trigger, 1);
    usleep(10);
    mraa_gpio_write(trigger, 0);
    while(mraa_gpio_read(echo) == 0){
        time1 = clock();
    }
    while(mraa_gpio_read(echo) == 1){
        time2 = clock()-time1;
    }
    if(time2>0&&time2<30000){
        distance = time2 / 58.82;
    }else{ 
        distance=0;//if get wrong distance
    }
    return distance;
}

void ThreadTask_Ultrasonic_read(){
    //long distance_l, distance_c, distance_r;                                       
    mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r;              
 
    signal(SIGINT, do_when_interrupted);
    trig_l = mraa_gpio_init(7);                                              
    echo_l = mraa_gpio_init(8);                                              
    trig_c = mraa_gpio_init(10);                                             
    echo_c = mraa_gpio_init(11);                                             
    trig_r = mraa_gpio_init(5);                                              
    echo_r = mraa_gpio_init(6);                                              
 
    if (trig_c == NULL || echo_c == NULL || trig_l == NULL || echo_l == NULLL || trig_r == NULL ||echo_r == NULL){                                            
       fprintf(stderr, "Failed to initialized.\n");
       return 1;
    }
        mraa_gpio_dir(trig_l, MRAA_GPIO_OUT);                                    
        mraa_gpio_dir(echo_l, MRAA_GPIO_IN);                                     
        mraa_gpio_dir(trig_c, MRAA_GPIO_OUT);                                    
        mraa_gpio_dir(echo_c, MRAA_GPIO_IN);                                     
        mraa_gpio_dir(trig_r, MRAA_GPIO_OUT);                                    
        mraa_gpio_dir(echo_r, MRAA_GPIO_IN);                                     
 
   while(isrunning == 1){
    usleep(20);
    distance_l = get_distance(trig_l, echo_l);
    usleep(20);
    distance_c = get_distance(trig_c, echo_c);
    usleep(20);
    distance_r = get_distance(trig_r, echo_r);                     
    //printf(" c:%d l:%d r:%d\n", distance_c, distance_l, distance_r);}}
}


/*
 *  This is for speed and drive control
 * */
void speed_control(mraa_pwm_context in1, mraa_pwm_context in2, float f_speed) {
	f_speed = f_speed / 100;                                  
        if (f_speed >= 0) {                                   
            mraa_pwm_write(in2, 1.0f);        
            mraa_pwm_write(in1, 1.0f - f_speed);
        }                                                   
        else if (f_speed < 0) {                               
            mraa_pwm_write(in1, 1.0f);        
            mraa_pwm_write(in2, 1.0f + f_speed);
        }    
}

void turn_left(){
    f_turn = CENTER - LEFT;
    mraa_pwm_write(turn_pwm, f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, f_speed);
    
}

void turn_right(){
    f_turn = CENTER - RIGHT;
    mraa_pwm_write(turn_pwm, f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, f_speed);
    
}

//void turn_direction(double d_degree){
    //f_turn = CENTER - RIGHT;
    //mraa_pwm_write(turn_pwm, f_turn);
    //usleep(10000);
    //speed_control(speed_pwm_in1, speed_pwm_in2, f_speed);
//}

void move_forward(){
    f_turn = CENTER;
    mraa_pwm_write(turn_pwm, f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, f_speed);
    
}

void move_backward(){
    f_turn = CENTER;
    mraa_pwm_write(turn_pwm, f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, f_speed);
}


/*
 * This part is for compute latitude and longitude distance
 * This two distance are computed separaterly
 * The latitude distance is for north and south
 * The longitude distance is for east and west
 */
double degree_to_rad(double d)
{
    return d * PI/180.0;
}

double get_latitude_distance(double d_lat1, double d_lat2, double d_lon1)
{
    double radLat1 = degree_to_rad(d_lat1);   
    double radLat2 = degree_to_rad(d_lat2);   
    double a = radLat1 - radLat2;   
    double b = degree_to_rad(d_lon1) - degree_to_rad(d_lon1);   
    double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));   
    s = s * EARTH_RADIUS;   
    s = round(s * 10000) / 10000;
    return s;

}

double get_longitude_distance(double d_lon1, double d_lon2, double d_lat1)
{
    double radLat1 = degree_to_rad(d_lat1);   
    double radLat2 = degree_to_rad(d_lat1);   
    double a = radLat1 - radLat2;   
    double b = degree_to_rad(d_lon1) - degree_to_rad(d_lon2);   
    double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));   
    s = s * EARTH_RADIUS;   
    s = round(s * 10000) / 10000;   
    return s;  
}


/*  This is for Collision avoide using Ultrasonic
 *  We set 0.5m as the distance threthold
 *  Case1: 
 *  all sensor l, c and r detecting obstacle. Then the car will move backwards and turn right/left
 *
 * */

int obstacle_case1(){
    int i;
    while(distance_l <= 4 || distance_r <= 4){
        move_backward();
    }
    for(i=0; i<= 100; i++){
        move_backward();                          //backward more to get space for turn
    }
    
    if(distance_l<=4 && distance_r >4){
        for(i=0; i<= 650; i++){
        turn_right();             
        }                 //
    }else if(distance_r<=4 && distance_l>4){
        for(i=0; i<= 650; i++){
        turn_left();             
        }                 //
    }else if(distance_r>4 && distance_l>4){
         if(d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=650;i++){
                     turn_right();
                 } 
        }else{   
            for (i = 0;i<=650;i++){
                     turn_left();
             }
        }
              //
    }

    for(i=0; i<= 100; i++){
        move_forward;                          
      }
    return 0;
}


/*  Case2:
 *  Only c sensor detecting obstacles. Then the car will turn right/left
 *
 * */
int obstacle_case2(){
    int i;
    if(distance_c <= 4){
     if(d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=650;i++){
                     turn_right();
                 } 
        }else
        {   
            for (i = 0;i<=650;i++){
                     turn_left();
             }
    }else{
        if(distance_slight_l <= 4 && distance_slight_r > 4){
            for(i=0; i<= 100; i++){
            turn_right();             
        }                               // turn slightly right

        }else if(distance_slight_r <=4 && distance_slight_l > 4){
            for(i=0; i<= 100; i++){
            turn_left();   
            }          
        }else if(distance_slight_r <=4 && distance_slight_r <= 4){
             while(distance_slight_l <= 4 || distance_slight_r <= 4){
                move_backward();
             }
             for(i=0; i<= 100; i++){
                 move_backward();                          //backward more to get space for turn
             }

            if(distance_slight_l<=4 && distance_slight_r >4){
                 for(i=0; i<= 650; i++){
                     turn_right();             
                 }                 //
            }else if(distance_slight_r<=4 && distance_slight_l>4){
                for(i=0; i<= 650; i++){
                     turn_left();             
                 }                 //
             }else if(distance_slight_r>4 && distance_slight_l>4){
                 if(d_move_direction - pT_drone->d_face_direction > 0){
                     for (i = 0;i<=650;i++){
                        turn_right();
                     } 
                 }else{   
                    for (i = 0;i<=650;i++){
                         turn_left();
                    }
                }
             }
        }
    }
    for(i=0; i<= 100; i++){
        move_forward();                          
    }
    return 0; 
}

/*  Case3:
 *  Only c and one of l,t sensor detecting obstacles. 
 *
 * */
int obstacle_case3(){
    int i;

    if(distance_l<=4 && distance_r >4){
        for(i=0; i<= 650; i++){
        turn_right();             
        }                 //
    }else if(distance_r<=4 && distance_l>4){
        for(i=0; i<= 650; i++){
        turn_left();             
        }                 //
    }else if(distance_r>4 && distance_l>4){
         if(d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=650;i++){
                     turn_right();
                 } 
        }else{   
            for (i = 0;i<=650;i++){
                     turn_left();
             }
        }
              //
    }

    for(i=0; i<= 100; i++){
        move_forward();                          
      }
    return 0;
}


int GpsNavigationMove(struct T_drone *pT_drone){
    int i;
    double d_west_to_east_distance, d_south_to_north_distance, d_move_direction;
    d_west_to_east_distance = get_latitude_distance(pT_drone->d_current_latitude, pT_drone->d_destination_latitude, pT_drone->d_current_longitude);
    d_south_to_north_distance = get_longitude_distance(pT_drone->d_current_longitude, pT_drone->d_destination_longitude, pT_drone->d_current_latitude);
    
    if ((pT_drone->d_destination_longitude - pT_drone->d_current_longitude) > 0)
    {
        d_move_direction = 90 - atan(d_south_to_north_distance / d_west_to_east_distance);
    }else if ((pT_drone->d_destination_longitude - pT_drone->d_current_longitude) < 0)
    {
        d_move_direction = 270 - atan(d_south_to_north_distance / d_west_to_east_distance);
    }else if ((pT_drone->d_destination_longitude - pT_drone->d_current_longitude) == 0)
    {
        if ((pT_drone->d_destination_latitude - pT_drone->d_current_latitude) >= 0)
        {
            d_move_direction = 0;
        }else if ((pT_drone->d_destination_latitude - pT_drone->d_current_latitude) < 0)
        {
            d_move_direction = 180;
        }
    }
    //printf("drone's location = lat:%f, lon:%f\n", pT_drone->d_current_latitude, pT_drone->d_current_longitude);
    //printf("Destination's location = lat:%f, lon:%f\n", pT_drone->d_destination_latitude, pT_drone->d_destination_longitude);
    //printf("d_move_direction = %f\n", d_move_direction);
    double tmp = abs(d_move_direction - pT_drone->d_face_direction);
    //printf("abs(d_move_direction - pT_drone->d_face_direction) = %f\n", tmp);
    //printf("pT_drone->d_face_direction = %f\n", pT_drone->d_face_direction);
    double tmp_distance = sqrt(pow(d_west_to_east_distance, 2) + pow(d_south_to_north_distance, 2));
    //printf("distance = %f\n\n", tmp_distance);
    if (tmp > 10)
    {
        //turn_direction(d_move_direction - pT_drone->d_face_direction);
        if(d_move_direction - pT_drone->d_face_direction > 0){
            turn_right();
        }else
        {   
            turn_left();
        }
    }else if (tmp_distance > 10)
    {
        printf("move forward\n");
        move_forward();
    }
    return 0;
}

void ThreadTask_GpsNavigationMove(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->n_stop_sign == 1)
        {
            break;
        }
        GpsNavigationMove(pT_drone);
        usleep(50000);
    }
}


