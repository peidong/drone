#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.048f
#define RIGHTMAX 0.088f

const double EARTH_RADIUS = 6378.137;

float f_speed, f_turn;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;

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
    printf("d_move_direction = %f\n", d_move_direction);
    double tmp = abs(d_move_direction - pT_drone->d_face_direction);
    printf("abs(d_move_direction - pT_drone->d_face_direction) = %f\n", tmp);
    printf("pT_drone->d_face_direction = %f\n", pT_drone->d_face_direction);
    double tmp_distance = sqrt(pow(d_west_to_east_distance, 2) + pow(d_south_to_north_distance, 2));
    printf("distance = %f\n\n", tmp_distance);
    if (tmp > 10)
    {
        //turn_direction(d_move_direction - pT_drone->d_face_direction);
        if(d_move_direction - pT_drone->d_face_direction > 0){
            for (i = 0;i<=180;i++){
                     turn_right();
                 } 
        }else
        {   
            for (i = 0;i<=180;i++){
                     turn_left();
                 }
        }
    }else if (tmp_distance > 10)
    {
        for (i = 0; i<=200; i++){
            printf("move forward\n");
            move_forward();
        }
    }
    return 0;
}

void ThreadTask_GpsNavigationMove(struct T_drone *pT_drone){
    int n_loop = 0;
    while(1){
        n_loop++;
        if (n_loop > 100)
        {
            break;
        }
        GpsNavigationMove(pT_drone);
        usleep(100000);
    }
}
