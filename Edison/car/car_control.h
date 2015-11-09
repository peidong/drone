#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.067f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.058f
#define RIGHTMAX 0.078f
//#define PRINT_DEBUG_THREAD

float speed, turn;
char speed_user_input[MAXBUFSIZ];
char turn_user_input[MAXBUFSIZ];
int i, case_num, averageTimes = 3, speed_flag = 1;
//double distance_l, distance_c, distance_r, distance_s_l, distance_s_r;
double distance_c, distance_s_l, distance_s_r;
//mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r, trig_up_l, echo_up_l, trig_up_r, echo_up_r;
mraa_gpio_context trig_c, echo_c;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm, sonicTurn_pwm;
 



double EARTH_RADIUS = 6378137;
float g_f_speed, g_f_turn;
//mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r; 
/**
 * This is for ultrasonic sensor read
 */
sig_atomic_t volatile isrunning =1;                                                                 
//void do_when_interrupted(int sig){
  //if (sig == SIGINT)
    //isrunning ==0;
//}

void ThreadTask_sonicTurn_pwm(struct T_drone *pT_drone){
    int i;
    float sonic_pwm;
    mraa_pwm_write(sonicTurn_pwm, 0.050f);
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
      for(i = 0; i <= 31; i++){
          sonic_pwm = CENTER + i * 0.001;
          mraa_pwm_write(sonicTurn_pwm, sonic_pwm);
          if(i > 3){
              pT_drone->n_ultrasonic_degree = 1;
          }
          //usleep(100000);
          usleep(20000);
      }
      mraa_pwm_write(sonicTurn_pwm, 0.050f);
      pT_drone->n_ultrasonic_degree = 0;
      for(i = 0; i <= 38; i++){
          sonic_pwm = CENTER - i * 0.001;
          mraa_pwm_write(sonicTurn_pwm, sonic_pwm);
          if(i > 3){
              pT_drone->n_ultrasonic_degree = -1;
          }

          usleep(20000);
          //usleep(100000);
      }
      mraa_pwm_write(sonicTurn_pwm, 0.050f);
      pT_drone->n_ultrasonic_degree = 0;
      usleep(20000);
    }
}


double get_distance(mraa_gpio_context trigger, mraa_gpio_context echo) 
{
  struct timeval startTime, endTime;
        double time_taken;
  double distance;

        mraa_gpio_write(trigger, 0);
        usleep(10);
        mraa_gpio_write(trigger, 1);
        usleep(20);
        mraa_gpio_write(trigger, 0);
  int count_while = 0;
  while (mraa_gpio_read(echo) == 0){
    count_while++;
    if (count_while > 300)
    {
      break;
    }
  }
  gettimeofday(&startTime, NULL);

  count_while = 0;
  while (mraa_gpio_read(echo) == 1){
    count_while++;
    if (count_while > 300)
    {
      break;
    }
  }
  gettimeofday(&endTime, NULL);
        
  time_taken = 1000000.0 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
        distance = (time_taken + 0.00) / 58.82;
        count_while = 0;
  while (time_taken < 30000 && time_taken > 0){
    gettimeofday(&endTime, NULL);
    time_taken = 1000000.0 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
    count_while++;
    // if (count_while > 100)
    // {
    //   break;
    // }

  }
  // printf("distance = %f\n", distance);
        return distance;
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

void do_case_1(struct T_drone *pT_drone ){
	int i;
    for(i=0; i<=200; i++){
        move_backward();
    }
    if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=550;i++){
                     turn_right();
                 } 
    }else{   
            for (i = 0;i<=550;i++){
                     turn_left();
             }
    }
    for(i=0; i<= 100; i++){
        move_forward();                          
      }
}

void do_case_2(){
    int i;
    for(i=0; i<=550; i++){
        turn_right();
    }

}

void do_case_3(){
    int i;
    for(i=0; i<=550; i++){
        turn_left();
    }
}

void do_case_4( struct T_drone *pT_drone){
    int i;
    if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=550;i++){
                     turn_right();
                 } 
    }else{   
            for (i = 0;i<=550;i++){
                     turn_left();
             }
    }
}

void do_case_5(){
  for(i=0; i<=350; i++){
        turn_right();
    }

}

void do_case_6(){
  for(i=0; i<=350; i++){
        turn_left();
    }

}

int case_detection(double s_left, double s_right, double Center){
	int returnVal;
	if (Center > 10 && Center <= 40 && s_left > 10 && s_left <= 40 && s_right > 10 && s_right <= 40)
		returnVal = 10;      //go back
    else if(Center > 10 && Center <=40 && s_left > 10 && s_left <=40 && s_right >10)
        returnVal = 2;     //turn right
    else if(Center > 10 && Center <= 40 && s_left >10 && s_right >10 && s_right <=40)
        returnVal = 3;      //turn left
    else if(Center > 10 && Center <= 40 && s_left >10 && s_right > 10)
        returnVal = 4;      //turn destination direction
    else if(Center > 10 && s_left <=40 && s_left > 10)
        returnVal = 5;      //turn slightly right
    else if(Center > 10 && s_right <= 40 && s_right > 10)
        returnVal = 6;      //turn slightly left
	else
		returnVal = 7;
	return returnVal;
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
    double tmp = abs(pT_drone->d_move_direction - pT_drone->d_face_direction);
    double tmp_distance = sqrt(pow(d_west_to_east_distance, 2) + pow(d_south_to_north_distance, 2));
    if (tmp > 15)
    {
        //turn_direction(pT_drone->d_move_direction - pT_drone->d_face_direction);
        if(pT_drone->d_move_direction - pT_drone->d_face_direction > 0){
            turn_right();
        }else if(pT_drone->d_move_direction - pT_drone->d_face_direction < 0)
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
	while (1){

    if (pT_drone->nflag_stop_all != 0)
        {
            break;
        }else if(pT_drone->n_control_type != 1){
            continue;
        }
    
        if(pT_drone->n_ultrasonic_degree == 0){
		    distance_c = get_distance(trig_c, echo_c);
        }else if(pT_drone->n_ultrasonic_degree == -1){
		distance_s_l = get_distance(trig_c, echo_c);
        }else if(pT_drone->n_ultrasonic_degree == 1){
		distance_s_r = get_distance(trig_c, echo_c);
        }
	
//        slow down when there is an obstacle near by.		

        //printf("%lf %lf %lf %lf %lf\n", distance_s_l, distance_s_r, distance_l, distance_c, distance_r);
        printf("%lf\t%lf\t%lf\n", distance_s_l, distance_s_r, distance_c);
            
        case_num = case_detection(distance_s_l, distance_s_r, distance_c);
        printf("case is %d\n", case_num);
            
        switch(case_num){
            case 1:
                do_case_1(pT_drone);
                break;
            case 2:
                do_case_2(); 
                break;
            case 3:
                do_case_3();
                break;
            case 4:
                do_case_4(pT_drone);
                break;
            case 5:
                do_case_5();
                break;
            case 6:
                do_case_6(); 
                break;
            default: 
                GpsNavigationMove(pT_drone);
                break;
        }
        // GpsNavigationMove(pT_drone);
	}
}
