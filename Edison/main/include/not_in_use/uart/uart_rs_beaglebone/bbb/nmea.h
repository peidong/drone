#ifndef _NMEA_H_
#define _NMEA_H_
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

struct gprmc {
    int state;
    double latitude;
    char lat;
    double longitude;
    char lon;
    // double speed;
    // double course;
};
typedef struct gprmc gprmc_t;

void nmea_parse_gprmc(char *, gprmc_t *);
void gps_convert_deg_to_dec(double *latitude, char ns,  double *longitude, char we);
double gps_deg_dec(double deg_point);


void nmea_parse_gprmc(char *nmea, gprmc_t *loc)
{
    char *p = nmea;

    // p = strchr(p, ',')+1; //skip time
    p = strchr(p, ',')+1; //status
    // printf("%c\n", p[0]);
    if(p[0]=='A')
        loc->state = 1;
    else if(p[0]=='V')
        loc->state = 0;
    else
        loc->state = 0;

    p = strchr(p, ',')+1;
    loc->latitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'N':
            loc->lat = 'N';
            break;
        case 'S':
            loc->lat = 'S';
            break;
        case ',':
            loc->lat = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    loc->longitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'W':
            loc->lon = 'W';
            break;
        case 'E':
            loc->lon = 'E';
            break;
        case ',':
            loc->lon = '\0';
            break;
    }
}

void gps_convert_deg_to_dec(double *latitude, char ns,  double *longitude, char we)
{
    double lat = (ns == 'N') ? *latitude : -1 * (*latitude);
    double lon = (we == 'E') ? *longitude : -1 * (*longitude);

    *latitude = gps_deg_dec(lat);
    *longitude = gps_deg_dec(lon);
}

double gps_deg_dec(double deg_point)
{
    double ddeg;
    double sec = modf(deg_point, &ddeg)*60;
    int deg = (int)(ddeg/100);
    int min = (int)(deg_point-(deg*100));

    double absdlat = round(deg * 1000000.);
    double absmlat = round(min * 1000000.);
    double absslat = round(sec * 1000000.);

    return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}

#endif
