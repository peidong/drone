#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.048f
#define RIGHTMAX 0.088f

const double EARTH_RADIUS = 6378137;
float g_f_speed, g_f_turn;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm, sonicTurn_pwm;


/**
 * This is for ultrasonic sensor read
 *
 */
// sig_atomic_t volatile isrunning =1;                                              
 
// void do_when_interrupted(int sig) {
//         if (sig == SIGINT)
//             isrunning = 0;
// }


void ThreadTask_sonicTurn_pwm(struct T_drone *pT_drone){
    int i;
    float sonic_pwm;
    mraa_pwm_write(sonicTurn_pwm, CENTER);
    pT_drone->n_ultrasonic_degree = 0;
    while(1){
      if (pT_drone->nflag_stop_all != 0){
          break;
      }else if(pT_drone->n_control_type != 1){
          continue;
      }
#ifdef PRINT_DEBUG_THREAD
      printf("ThreadTask_sonicTurn_pwm\n");
#endif
      for(i = 0; i <= 30; i++){
          sonic_pwm = CENTER + i * 0.001;
          mraa_pwm_write(sonicTurn_pwm, sonic_pwm);
          if(i > 3){
              pT_drone->n_ultrasonic_degree = 1;
          }
          usleep(100000);
      }
      mraa_pwm_write(sonicTurn_pwm, CENTER);
      pT_drone->n_ultrasonic_degree = 0;
      for(i = 0; i <= 30; i++){
          sonic_pwm = CENTER - i * 0.001;
          mraa_pwm_write(sonicTurn_pwm, sonic_pwm);
          if(i > 3){
              pT_drone->n_ultrasonic_degree = -1;
          }

          usleep(100000);
      }
      mraa_pwm_write(sonicTurn_pwm, CENTER);
      pT_drone->n_ultrasonic_degree = 0;
      usleep(100000);
    }
}

long get_distance(mraa_gpio_context trigger, mraa_gpio_context echo)
{ 
    long distance, time1, time2;
    mraa_gpio_write(trigger, 0);
    printf("get_distance 1\n");
    usleep(10);
    mraa_gpio_write(trigger, 1);
    usleep(10);
    mraa_gpio_write(trigger, 0);
    printf("get_distance 2\n");
    while(mraa_gpio_read(echo) == 0){
        time1 = clock();
        printf("get_distance 3\n");
    }
    while(mraa_gpio_read(echo) == 1){
        time2 = clock()-time1;
        printf("get_distance 4\n");
    }
    if(time2>0&&time2<30000){
        distance = time2 / 58.82;
        printf("get_distance 5\n");
    }else{ 
        distance=-1;//if get wrong distance
        printf("get_distance 6\n");
    }
    return distance;
}

void ThreadTask_Ultrasonic_turn(){
    int i = 0;
    

}

void ThreadTask_Ultrasonic_read(struct T_drone *pT_drone){
    //long pT_drone->ln_distance_left, pT_drone->ln_distance_center, pT_drone->ln_distance_right;                                       
    mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r;              
 
    // signal(SIGINT, do_when_interrupted);
    trig_l = mraa_gpio_init(7);                                          
    echo_l = mraa_gpio_init(8);                                     
    trig_c = mraa_gpio_init(10);                                             
    echo_c = mraa_gpio_init(11);                                             
    trig_r = mraa_gpio_init(13);                                              
    echo_r = mraa_gpio_init(12);                                              
 
    if (trig_c == NULL || echo_c == NULL || trig_l == NULL || echo_l == NULL || trig_r == NULL ||echo_r == NULL){                                            
       fprintf(stderr, "Failed to initialized.\n");
       return;
    }
        mraa_gpio_dir(trig_l, MRAA_GPIO_OUT);                                    
        mraa_gpio_dir(echo_l, MRAA_GPIO_IN);                                     
        mraa_gpio_dir(trig_c, MRAA_GPIO_OUT);                                    
        mraa_gpio_dir(echo_c, MRAA_GPIO_IN);                                     
        mraa_gpio_dir(trig_r, MRAA_GPIO_OUT);                                    
        mraa_gpio_dir(echo_r, MRAA_GPIO_IN);                                     
 
    
   while(1){
   // while(isrunning == 1){
    if (pT_drone->nflag_stop_all != 0){
      break;
    }else if(pT_drone->n_control_type != 1){
      continue;
    }
#ifdef PRINT_DEBUG_THREAD
      printf("ThreadTask_Ultrasonic_read\n");
#endif
    usleep(20);
    printf("ultrasonic 1\n");
    pT_drone->ln_distance_left = get_distance(trig_l, echo_l);
    printf("ultrasonic 2\n");
    usleep(20);
    if(pT_drone->n_ultrasonic_degree == 0){
        pT_drone->ln_distance_center = get_distance(trig_c, echo_c);
    }else if(pT_drone->n_ultrasonic_degree == 1){
        pT_drone->ln_distance_slight_right = get_distance(trig_c, echo_c);
    }else if(pT_drone->n_ultrasonic_degree == -1){
        pT_drone->ln_distance_slight_left = get_distance(trig_c, echo_c);
    }
    printf("ultrasonic 3\n");
    usleep(20);
    pT_drone->ln_distance_right = get_distance(trig_r, echo_r);
    printf("ultrasonic 4\n");                     
    //printf(" c:%d l:%d r:%d\n", pT_drone->ln_distance_center, pT_drone->ln_distance_left, pT_drone->ln_distance_right);}}
    }
}

/*
 *  This is for speed and drive control
 * */
void speed_control(mraa_pwm_context in1, mraa_pwm_context in2, float g_f_speed) {
	g_f_speed = g_f_speed / 100;                                  
        if (g_f_speed >= 0) {                                   
            mraa_pwm_write(in2, 1.0f);        
            mraa_pwm_write(in1, 1.0f - g_f_speed);
        }                                                   
        else if (g_f_speed < 0) {                               
            mraa_pwm_write(in1, 1.0f);        
            mraa_pwm_write(in2, 1.0f + g_f_speed);
        }    
}

void turn_left(){
    g_f_turn = CENTER - LEFT;
    mraa_pwm_write(turn_pwm, g_f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, g_f_speed);
    
}

void turn_right(){
    g_f_turn = CENTER - RIGHT;
    mraa_pwm_write(turn_pwm, g_f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, g_f_speed);
    
}

//void turn_direction(double d_degree){
    //g_f_turn = CENTER - RIGHT;
    //mraa_pwm_write(turn_pwm, g_f_turn);
    //usleep(10000);
    //speed_control(speed_pwm_in1, speed_pwm_in2, g_f_speed);
//}

void move_forward(){
    g_f_turn = CENTER;
    mraa_pwm_write(turn_pwm, g_f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, g_f_speed);
    
}

void move_backward(){
    g_f_turn = CENTER;
    mraa_pwm_write(turn_pwm, g_f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, (-g_f_speed));
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

int obstacle_case1(struct T_drone *pT_drone){
    int i;
    while(pT_drone->ln_distance_left <= 60 || pT_drone->ln_distance_right <= 60){
        move_backward();
    }
    for(i=0; i<= 100; i++){
        move_backward();                          //backward more to get space for turn
    }
    
    if(pT_drone->ln_distance_left<=60 && pT_drone->ln_distance_right >60){
        for(i=0; i<= 650; i++){
        turn_right();             
        }                 //
    }else if(pT_drone->ln_distance_right<=60 && pT_drone->ln_distance_left>60){
        for(i=0; i<= 650; i++){
        turn_left();             
        }                 //
    }else if(pT_drone->ln_distance_right>60 && pT_drone->ln_distance_left>60){
         if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
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
int obstacle_case2(struct T_drone *pT_drone){
    int i;
    if(pT_drone->ln_distance_center <= 60){
        if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=650;i++){
                     turn_right();
                 } 
        }else{   
            for (i = 0;i<=650;i++){
                     turn_left();
             }
    }
    
    }else{
        if(pT_drone->ln_distance_slight_left <= 60 && pT_drone->ln_distance_slight_right > 60){
            for(i=0; i<= 100; i++){
            turn_right();             
        }                               // turn slightly right

        }else if(pT_drone->ln_distance_slight_right <= 60 && pT_drone->ln_distance_slight_left > 60){
            for(i=0; i<= 100; i++){
            turn_left();   
            }          
        }else if(pT_drone->ln_distance_slight_right <= 60 && pT_drone->ln_distance_slight_right <= 60){
             while(pT_drone->ln_distance_slight_left <= 60 || pT_drone->ln_distance_slight_right <= 60){
                move_backward();
             }
             for(i=0; i<= 100; i++){
                 move_backward();                          //backward more to get space for turn
             }

            if(pT_drone->ln_distance_slight_left<= 60 && pT_drone->ln_distance_slight_right > 60){
                 for(i=0; i<= 650; i++){
                     turn_right();             
                 }                 //
            }else if(pT_drone->ln_distance_slight_right<=60 && pT_drone->ln_distance_slight_left> 60){
                for(i=0; i<= 650; i++){
                     turn_left();             
                 }                 //
             }else if(pT_drone->ln_distance_slight_right > 60 && pT_drone->ln_distance_slight_left> 60){
                 if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
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
int obstacle_case3(struct T_drone *pT_drone){
    int i;

    if(pT_drone->ln_distance_left<=60 && pT_drone->ln_distance_right >60){
        for(i=0; i<= 650; i++){
        turn_right();             
        }                 //
    }else if(pT_drone->ln_distance_right<=60 && pT_drone->ln_distance_left>60){
        for(i=0; i<= 650; i++){
        turn_left();             
        }                 //
    }else if(pT_drone->ln_distance_right>60 && pT_drone->ln_distance_left>60){
         if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
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
    double d_west_to_east_distance, d_south_to_north_distance;
    d_west_to_east_distance = get_latitude_distance(pT_drone->d_current_latitude, pT_drone->d_destination_latitude, pT_drone->d_current_longitude);
    d_south_to_north_distance = get_longitude_distance(pT_drone->d_current_longitude, pT_drone->d_destination_longitude, pT_drone->d_current_latitude);
    
    if ((pT_drone->d_destination_longitude - pT_drone->d_current_longitude) > 0)
    {
        pT_drone->d_move_direction = 90 - atan(d_south_to_north_distance / d_west_to_east_distance);
    }else if ((pT_drone->d_destination_longitude - pT_drone->d_current_longitude) < 0)
    {
        pT_drone->d_move_direction = 270 - atan(d_south_to_north_distance / d_west_to_east_distance);
    }else if ((pT_drone->d_destination_longitude - pT_drone->d_current_longitude) == 0)
    {
        if ((pT_drone->d_destination_latitude - pT_drone->d_current_latitude) >= 0)
        {
            pT_drone->d_move_direction = 0;
        }else if ((pT_drone->d_destination_latitude - pT_drone->d_current_latitude) < 0)
        {
            pT_drone->d_move_direction = 180;
        }
    }
    //printf("drone's location = lat:%f, lon:%f\n", pT_drone->d_current_latitude, pT_drone->d_current_longitude);
    //printf("Destination's location = lat:%f, lon:%f\n", pT_drone->d_destination_latitude, pT_drone->d_destination_longitude);
    //printf("pT_drone->d_move_direction = %f\n", pT_drone->d_move_direction);
    double tmp = abs(pT_drone->d_move_direction - pT_drone->d_face_direction);
    //printf("abs(pT_drone->d_move_direction - pT_drone->d_face_direction) = %f\n", tmp);
    //printf("pT_drone->d_face_direction = %f\n", pT_drone->d_face_direction);
    double tmp_distance = sqrt(pow(d_west_to_east_distance, 2) + pow(d_south_to_north_distance, 2));
    //printf("distance = %f\n\n", tmp_distance);
    if (tmp > 10)
    {
        //turn_direction(pT_drone->d_move_direction - pT_drone->d_face_direction);
        if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
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

void ThreadTask_manual_control(struct T_drone *pT_drone){
    int i; 
    while(1){
      if (pT_drone->nflag_stop_all != 0){
          break;
      }else if(pT_drone->n_control_type != 2){
          continue;
      }
#ifdef PRINT_DEBUG_THREAD
      printf("ThreadTask_manual_control\n");
#endif
    // left is 6; right is 7; moveforward is 4; backward is 5; stop is  
#ifdef PRINT_CAR_MANUAL
      printf("pT_drone->n_manual_control_command = %d\n", pT_drone->n_manual_control_command);
#endif
      if (pT_drone->n_manual_control_command == 6)     
      {
         for (i = 0;i<=90;i++){
             turn_left();
         }
      }else if (pT_drone->n_manual_control_command == 4)
      {
         for (i = 0;i<=50;i++){
             move_forward();
         }
      }               
      else if (pT_drone->n_manual_control_command == 7){
         for (i = 0;i<=90;i++){
             turn_right();
         } 
      }
      else if(pT_drone->n_manual_control_command == 10)
      {
          g_f_turn = CENTER;
          mraa_pwm_write(turn_pwm, g_f_turn);
          usleep(10000);
          speed_control(speed_pwm_in1, speed_pwm_in2, 0);
      }
      else if (pT_drone->n_manual_control_command == 5){
          for (i = 0;i<=50;i++){
             move_backward();
         }
      }else{                                 
             printf("Wrong turn type!\n");
     }
  
      usleep(1000);
      speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
  }
}

void ThreadTask_GpsNavigationMove(struct T_drone *pT_drone){
    while(1){
        if (pT_drone->nflag_stop_all != 0)
        {
            break;
        }else if(pT_drone->n_control_type != 1){
            continue;
        }
#ifdef PRINT_DEBUG_THREAD
      printf("ThreadTask_GpsNavigationMove\n");
#endif
        if(pT_drone->ln_distance_left <= 60 && pT_drone->ln_distance_right <= 60 && pT_drone->ln_distance_center <=60){
            obstacle_case1(&g_T_drone_self);
        }else if(pT_drone->ln_distance_left > 60 && pT_drone->ln_distance_right > 60){
            if(pT_drone->ln_distance_center <= 60 || pT_drone->ln_distance_slight_left <= 60 || pT_drone->ln_distance_slight_right <= 60){
                obstacle_case2(&g_T_drone_self);
            }
        }else if(pT_drone->ln_distance_left <= 60 || pT_drone->ln_distance_right <= 60 && pT_drone->ln_distance_center <= 60){
            obstacle_case3(&g_T_drone_self); 
        }else{
             GpsNavigationMove(pT_drone);
        }
        usleep(50000);
    }
}