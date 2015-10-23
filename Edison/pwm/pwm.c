#include <mraa/pwm.h>
#include "../include/drone_include.h"
int main()
{
    mraa_pwm_context pwm;
    pwm = mraa_pwm_init(3);
    if (pwm == NULL) {
        return 1;
    }
    struct T_pwm *pT_pwm_all;
    double arrd_pwm[4];
    char *arrsz_pwm_key_name[4] = {"pwm1", "pwm2", "pwm3", "pwm4"};
    int n_index;
    pT_pwm_all = GetPwmStruct();
    for(n_index = 0; n_index < 4; n_index++){
        arrd_pwm[n_index] = GetPwmValue(pT_pwm_all, arrsz_pwm_key_name[n_index]);
        printf("%s's value is %f\n", arrsz_pwm_key_name[n_index], arrd_pwm[n_index]);
    }

    mraa_pwm_period_us(pwm, 200);
    mraa_pwm_enable(pwm, 1);
    float value = 0.0f;
    while (1) {
        value = value + 0.01f;
        mraa_pwm_write(pwm, value);
        usleep(50000);
        if (value >= 1.0f) {
            value = 0.0f;
        }
        float output = mraa_pwm_read(pwm);
    }
    return 0;
}
