#include "car_control.h"

const double EARTH_RADIUS = 6378.137;

static double rad(double d)
{
    return d * PI/180.0;
}

static double GetLatDistance(double lat1, double lat2)
{
    double radLat1 = rad(lat1);   
    double radLat2 = rad(lat2);   
    double a = radLat1 - radLat2;   
    double b = rad(0) - rad(0);   
    double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));   
    s = s * EARTH_RADIUS;   
    s = round(s * 10000) / 10000;   
    return s;   

}

static double GetLonDistance(double lon1, double lon2)
{
    double radLat1 = rad(0);   
    double radLat2 = rad(0);   
    double a = radLat1 - radLat2;   
    double b = rad(lon1) - rad(lon2);   
    double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));   
    s = s * EARTH_RADIUS;   
    s = round(s * 10000) / 10000;   
    return s;  
}
