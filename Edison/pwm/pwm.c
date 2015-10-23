#include <mraa/pwm.h>
#include "../include/drone_include.h"
int main()
{
    mraa_pwm_context pwm;
    pwm = mraa_pwm_init(3);
    if (pwm == NULL) {
        return 1;
    }
    mraa_pwm_period_us(pwm, 200);
    mraa_pwm_enable(pwm, 1);
    char *arrsz_pwm_key_name[4] = {"pwm1", "pwm2", "pwm3", "pwm4"};
    int n_index;
    while(1)
    {
        g_pT_pwm = get_pT_pwm();
        g_arrd_pwm[0] = get_d_pwm(g_pT_pwm, "pwm1") / 100;
        mraa_pwm_write(pwm, (g_arrd_pwm[0]/100));
        usleep(50000);
        printf("pwm1 = %f\n", (g_arrd_pwm[0]/100));
        float output = mraa_pwm_read(pwm);
    }
    return 0;
}
