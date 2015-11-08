#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.058f
#define RIGHTMAX 0.078f
// #define PRINT_DEBUG_THREAD

float speed, turn;
char speed_user_input[MAXBUFSIZ];
char turn_user_input[MAXBUFSIZ];
int i, case_num, averageTimes = 3, speed_flag = 1;
double distance_l, distance_c, distance_r, distance_s_l, distance_s_r;
mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r, trig_up_l, echo_up_l, trig_up_r, echo_up_r;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm, sonicTurn_pwm;
 



double EARTH_RADIUS = 6378137;
float g_f_speed, g_f_turn;
//mraa_gpio_context trig_l, echo_l, trig_c, echo_c, trig_r, echo_r; 
double n_ultrasonic_degree;
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


double get_distance(mraa_gpio_context trigger, mraa_gpio_context echo) 
{
  struct timeval startTime, endTime;
        double time_taken;
  double distance;

        mraa_gpio_write(trigger, 0);
        usleep(5);
        mraa_gpio_write(trigger, 1);
        usleep(11);
        mraa_gpio_write(trigger, 0);
  
  while (mraa_gpio_read(echo) == 0);
  gettimeofday(&startTime, NULL);
  
  while (mraa_gpio_read(echo) == 1);
  gettimeofday(&endTime, NULL);
        
  time_taken = 1000000.0 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
        distance = (time_taken + 0.00) / 58.82;
  while (time_taken < 30000 && time_taken > 0){
    gettimeofday(&endTime, NULL);
    time_taken = 1000000.0 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
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

void do_case_1(double left, double right, mraa_pwm_context turn, mraa_pwm_context in1, mraa_pwm_context in2){
	speed_control(in1, in2, -100);
	sleep(1);
	speed_control(in1, in2, 100);
	if ((left - right) > 0.00001)
		mraa_pwm_write(turn, LEFT);
	else
		mraa_pwm_write(turn, RIGHT);
	sleep(1);
	usleep(500000);
}

void do_case_2(double left, mraa_pwm_context turn, mraa_gpio_context trig, mraa_gpio_context echo){
	int counter = 0;
	double previous = 1.0, current = 0, dutyCycle;
	while (counter < 3){
		while (current <= previous){
			counter = 0;
			previous = get_distance(trig, echo);
			dutyCycle = mraa_pwm_read(turn);
			dutyCycle += 0.005f;
			if (dutyCycle < RIGHTMAX)
				mraa_pwm_write(turn, dutyCycle);
			usleep(200000);
			current = get_distance(trig, echo);
			if (current > 80) break;
		}
		previous = current;
		usleep(150000);
		current = get_distance(trig, echo);
		if (current > 80)
			break;
		if (current > previous)
			counter += 1;
	}
	mraa_pwm_write(turn, CENTER);
}

void do_case_3(double right, mraa_pwm_context turn, mraa_gpio_context trig, mraa_gpio_context echo){
	int counter = 0;
	double previous = 0, current, dutyCycle;
	current = right;
	while (counter < 3){
		while (current <= previous){
			counter = 0;
			previous = get_distance(trig, echo);
			dutyCycle = mraa_pwm_read(turn);
			dutyCycle -= 0.005f;
			if (dutyCycle > LEFTMAX){
				mraa_pwm_write(turn, dutyCycle);
			}
			usleep(200000);
			current = get_distance(trig, echo);
			if (current > 80) break;
		}
		previous = current;
		usleep(150000);
		current = get_distance(trig, echo);
		if (current > 80)
			break;
		if (current > previous)
			counter += 1;
	}
	mraa_pwm_write(turn, CENTER);
}

void do_case_4(mraa_pwm_context turn, mraa_gpio_context trig_l, mraa_gpio_context echo_l){
	mraa_pwm_write(turn, RIGHT);
	usleep(600000);
	//sleep(1);
	double left = get_distance(trig_l, echo_l);
	do_case_2(left, turn, trig_l, echo_l);
}

void do_case_5(mraa_pwm_context turn, mraa_gpio_context trig_r, mraa_gpio_context echo_r){
	mraa_pwm_write(turn, LEFT);
	usleep(600000);
	//sleep(1);
	double right = get_distance(trig_r, echo_r);
	do_case_3(right, turn, trig_r, echo_r);
}

void do_case_6(mraa_pwm_context in1, mraa_pwm_context in2, mraa_pwm_context turn){
	mraa_pwm_write(turn, CENTER);
	speed_control(in1, in2, -100.0f);
	sleep(2);
	u_turn(in1, in2, turn);
}

void do_case_7(mraa_pwm_context in1, mraa_pwm_context in2, mraa_pwm_context turn, int speed_flag){
	if (speed_flag == 0){
		speed_flag = 1;
		mraa_pwm_write(turn, CENTER);
		speed_control(in1, in2, 100.0f);
	}
}

int case_detection(double s_left, double s_right, double left, double Center, double right){
	int returnVal;
	if (s_left < 40 && s_right < 40 && s_right > 10)
		return 6;
	else if (s_left < 40 && s_right > 50)
		return 4;
	else if (s_left > 50 && s_right < 40 && s_right > 10)
		return 5;
	if (Center <= 40 && left > 50 && right > 50)
		returnVal = 1;
	else if (Center > 50 && left <= 30 && right > 50)
		returnVal = 2;
	else if (Center > 50 && left > 50 && right <= 30)
		returnVal = 3;
	else if (Center <= 50 && left <= 50 && right <= 50)
		returnVal = 6;
	else if (Center <= 50 && left <= 50 && right > 50)
		returnVal = 4;
	else if (Center <= 50 && left > 50 && right <= 50)
		returnVal = 5;
	else
		returnVal = 7;
	return returnVal;
}

void u_turn(mraa_pwm_context in1, mraa_pwm_context in2, mraa_pwm_context turn) {
	mraa_pwm_write(turn, 0.058f);
	usleep(100000);
	speed_control(in1, in2, -70);
	sleep(1);

	mraa_pwm_write(turn, 0.074f);
	usleep(100000);
	speed_control(in1, in2, 100);
	sleep(1);

    	mraa_pwm_write(in1, 1.0f);                                                        
    	mraa_pwm_write(in2, 1.0f); 
	usleep(100000);

	mraa_pwm_write(turn, 0.058f);
	usleep(100000);
	speed_control(in1, in2, -100);
	sleep(1);

	mraa_pwm_write(in1, 1.0f);                                    
    	mraa_pwm_write(in2, 1.0f); 
	usleep(100000);	

	mraa_pwm_write(turn, 0.074f);                                                     
    	usleep(100000);                                                                   
    	speed_control(in1, in2, 100);                                                     
    	sleep(1);                                                                         
    	mraa_pwm_write(in1, 1.0f);                                                        
    	mraa_pwm_write(in2, 1.0f);                                                        
    	usleep(100000);                                                                   
    	mraa_pwm_write(turn, 0.058f);                                                     
    	usleep(100000);                                                                   
    	speed_control(in1, in2, -57);                                                    
    	sleep(1);                                                                         
    	mraa_pwm_write(in1, 1.0f);                                                        
    	mraa_pwm_write(in2, 1.0f);                                                        
    	usleep(100000);  	

	mraa_pwm_write(turn, CENTER);
	speed_control(in1, in2, 100);
	usleep(100000);	
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
	while (isrunning == 1){

    if (pT_drone->nflag_stop_all != 0)
        {
            break;
        }else if(pT_drone->n_control_type != 1){
            continue;
        }
    
        if(n_ultrasonic_degree == 0){
		    distance_c = get_distance(trig_c, echo_c);
        }else if(n_ultrasonic_degree == -1){
		distance_s_l = get_distance(trig_c, echo_c);
        }else if(n_ultrasonic_degree == 1){
		distance_s_r = get_distance(trig_c, echo_c);
        }
		distance_c = get_distance(trig_c, echo_c);
		distance_r = get_distance(trig_r, echo_r);
	
		//slow down when there is an obstacle near by.		
		if (distance_s_l < 70 || (distance_s_r < 70 && distance_s_r > 10)|| distance_l < 50 || distance_c < 50 || distance_r < 50){
			speed_flag = 0;
			speed_control(speed_pwm_in1, speed_pwm_in2, 70);
		}

		printf("%lf %lf %lf %lf %lf\n", distance_s_l, distance_s_r, distance_l, distance_c, distance_r);
			
		case_num = case_detection(distance_s_l, distance_s_r, distance_l, distance_c, distance_r);
		//printf("case is %d\n", case_num);
			
		switch(case_num){
			case 1:
				do_case_1(distance_l, distance_r, turn_pwm, speed_pwm_in1, speed_pwm_in2);
				break;
			case 2:
				do_case_2(distance_l, turn_pwm, trig_l, echo_l); 
				break;
			case 3:
				do_case_3(distance_r, turn_pwm, trig_r, echo_r);
				break;
			case 4:
				do_case_4(turn_pwm, trig_l, echo_l);
				break;
			case 5:
				do_case_5(turn_pwm, trig_r, echo_r);
				break;
			case 6:
				do_case_6(speed_pwm_in1, speed_pwm_in2, turn_pwm);
				break;
			default: 
				do_case_7(speed_pwm_in1, speed_pwm_in2, turn_pwm, speed_flag);
				break;
		}
	}


    }
