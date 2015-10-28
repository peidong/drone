#include <mraa/pwm.h>
#include "../../main/include/drone_include.h"
int main()
{
    double g_arrd_last_pwm[4] = {0.0, 0.0, 0.0 ,0.0};
    mraa_pwm_context pwm1;
    mraa_pwm_context pwm2;
    mraa_pwm_context pwm3;
    mraa_pwm_context pwm4;
    pwm1 = mraa_pwm_init(3);
    pwm2 = mraa_pwm_init(5);
    pwm3 = mraa_pwm_init(6);
    pwm4 = mraa_pwm_init(9);
    if (pwm1 == NULL || pwm2 == NULL || pwm3 == NULL || pwm4 == NULL) {
        return 1;
    }
    mraa_pwm_period_us(pwm1, 20000);
    mraa_pwm_enable(pwm1, 1);
    mraa_pwm_period_us(pwm2, 20000);
    mraa_pwm_enable(pwm2, 1);
    mraa_pwm_period_us(pwm3, 20000);
    mraa_pwm_enable(pwm3, 1);
    mraa_pwm_period_us(pwm4, 20000);
    mraa_pwm_enable(pwm4, 1);
    char *arrsz_pwm_key_name[4] = {"pwm1", "pwm2", "pwm3", "pwm4"};
    int n_index;
    while(1)
    {
        HTTP_update_T_drone_pwm(&g_T_drone_my);
        g_T_drone_my.arrd_current_pwm[0] = get_d_pwm(g_pT_hash_pwm, "pwm1") / 1000;
        g_T_drone_my.arrd_current_pwm[1] = get_d_pwm(g_pT_hash_pwm, "pwm2") / 1000;
        g_T_drone_my.arrd_current_pwm[2] = get_d_pwm(g_pT_hash_pwm, "pwm3") / 1000;
        g_T_drone_my.arrd_current_pwm[3] = get_d_pwm(g_pT_hash_pwm, "pwm4") / 1000;
        if (g_arrd_last_pwm[0] != g_T_drone_my.arrd_current_pwm[0]){
            mraa_pwm_write(pwm1, (g_T_drone_my.arrd_current_pwm[0]));
        }
        if (g_arrd_last_pwm[1] != g_T_drone_my.arrd_current_pwm[1]){
            mraa_pwm_write(pwm2, (g_T_drone_my.arrd_current_pwm[1]));
        }
        if (g_arrd_last_pwm[2] != g_T_drone_my.arrd_current_pwm[2]){
            mraa_pwm_write(pwm3, (g_T_drone_my.arrd_current_pwm[2]));
        }
        if (g_arrd_last_pwm[3] != g_T_drone_my.arrd_current_pwm[3]){
            mraa_pwm_write(pwm4, (g_T_drone_my.arrd_current_pwm[3]));
        }
        g_arrd_last_pwm[0] = g_T_drone_my.arrd_current_pwm[0];
        g_arrd_last_pwm[1] = g_T_drone_my.arrd_current_pwm[1];
        g_arrd_last_pwm[2] = g_T_drone_my.arrd_current_pwm[2];
        g_arrd_last_pwm[3] = g_T_drone_my.arrd_current_pwm[3];
        usleep(50000);
        printf("pwm1 = %f\n", (g_T_drone_my.arrd_current_pwm[0]));
        printf("pwm2 = %f\n", (g_T_drone_my.arrd_current_pwm[1]));
        printf("pwm3 = %f\n", (g_T_drone_my.arrd_current_pwm[2]));
        printf("pwm4 = %f\n\n", (g_T_drone_my.arrd_current_pwm[3]));
        /*float output = mraa_pwm_read(pwm1);*/
    }
    return 0;
}
