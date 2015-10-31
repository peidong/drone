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

void turn_direction(double d_degree){
    f_turn = CENTER - RIGHT;
    mraa_pwm_write(turn_pwm, f_turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, f_speed);
}

void move_forward(double d_distance){
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
    double d_west_to_east_distance, d_south_to_north_distance, d_move_direction;
    d_west_to_east_distance = get_latitude_distance(pT_drone->d_current_latitude, pT_drone->d_destination_latitude, pT_drone->d_current_longitude);
    d_south_to_north_distance = get_longitude_distance(pT_drone->d_current_longitude, pT_drone->d_destination_longitude, pT_drone->d_current_latitude);
    
    if (d_west_to_east_distance > 0)
    {
        d_move_direction = 90 - atan(d_south_to_north_distance / d_west_to_east_distance);
    }else if (d_west_to_east_distance < 0)
    {
        d_move_direction = 270 - atan(d_south_to_north_distance / d_west_to_east_distance);
    }else if (d_west_to_east_distance == 0)
    {
        if (d_south_to_north_distance >= 0)
        {
            d_move_direction = 0;
        }else if (d_south_to_north_distance < 0)
        {
            d_move_direction = 180;
        }
    }

    if (d_move_direction - pT_drone->face_direction > 10)
    {
        turn_direction(d_move_direction - pT_drone->face_direction);
    }
    if (sqrt(pow(d_west_to_east_distance, 2) + pow(d_south_to_north_distance, 2)) > 3)
    {
        move_forward(sqrt(pow(d_west_to_east_distance, 2) + pow(d_south_to_north_distance, 2)));
    }
    return 0;
}

void ThreadTask_GpsNavigationMove(struct T_drone *pT_drone){
    while(1){
        GpsNavigationMove(pT_drone);
    }
}