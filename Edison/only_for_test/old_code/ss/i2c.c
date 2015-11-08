#include <stdio.h>
#include <unistd.h>
#include <mraa.h>

int main()
{
    uint8_t m[2];
    m[0]=8;
    m[1]=255;
    mraa_init(); // can we put it in the beginning. Avoid repeating definition.
    mraa_i2c_context pwm12;
    pwm12 = mraa_i2c_init(2);
    mraa_i2c_address(pwm12,12);
    while(1)
    mraa_i2c_write(pwm12,m,2);
    return 0;
}
    