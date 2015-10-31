#include "../main/include/drone_include.h"


#define MAXBUFSIZ 1024
#define CENTER 0.068f
#define LEFT 0.015f
#define RIGHT -0.015f
#define LEFTMAX 0.048f
#define RIGHTMAX 0.088f

const double EARTH_RADIUS = 6378.137;

float speed, turn;
mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;

void speed_control(mraa_pwm_context in1, mraa_pwm_context in2, float speed) {
	speed = speed / 100;                                  
        if (speed >= 0) {                                   
            mraa_pwm_write(in2, 1.0f);        
            mraa_pwm_write(in1, 1.0f - speed);
        }                                                   
        else if (speed < 0) {                               
            mraa_pwm_write(in1, 1.0f);        
            mraa_pwm_write(in2, 1.0f + speed);
        }    
}

void turn_left(){
    turn = CENTER - LEFT;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}

void turn_right(){
    turn = CENTER - RIGHT;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}

void move_forward(){
    turn = CENTER;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}

void move_backward(){
    turn = CENTER;
    mraa_pwm_write(turn_pwm, turn);
    usleep(10000);
    speed_control(speed_pwm_in1, speed_pwm_in2, speed);
    
}


/*
 * This part is for compute latitude and longitude distance
 * This two distance are computed separaterly
 * The latitude distance is for north and south
 * The longitude distance is for east and west
 */
static double rad(double d)
{
    return d * PI/180.0;
}

static double GetLatDistance(double lat1, double lat2, double lon1)
{
    double radLat1 = rad(lat1);   
    double radLat2 = rad(lat2);   
    double a = radLat1 - radLat2;   
    double b = rad(lon1) - rad(lon1);   
    double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));   
    s = s * EARTH_RADIUS;   
    s = round(s * 10000) / 10000;   
    return s;   

}

static double GetLonDistance(double lon1, double lon2, double lat1)
{
    double radLat1 = rad(lat1);   
    double radLat2 = rad(lat1);   
    double a = radLat1 - radLat2;   
    double b = rad(lon1) - rad(lon2);   
    double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));   
    s = s * EARTH_RADIUS;   
    s = round(s * 10000) / 10000;   
    return s;  
}

