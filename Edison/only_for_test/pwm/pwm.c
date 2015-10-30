#include <mraa/pwm.h>
#include <stdint.h>
#include "../../main/include/drone_include.h"
int main()
{
	mraa_init(); // can we put it in the beginning. Avoid repeating definition.
	mraa_i2c_context pwm12, pwm34;
	pwm12 = mraa_i2c_init(1);
	// pwm34 = mraa_i2c_init(2);
	mraa_i2c_address(pwm12, 2); // i2c address 2. It can be arbitrarily defined. For pwm12 output, address = 2
	//mraa_i2c_address(pwm34, 3); // i2c address 3. It can be arbitrarily defined. For pwm34 output, address = 3
	double arrd_current_duty[4];
	uint8_t arri_i2c_output[4] = { 0, 0, 0, 0 };

	char *arrsz_pwm_key_name[4] = { "pwm1", "pwm2", "pwm3", "pwm4" };
	int n_index;
	while (1)
	{
		update_T_drone_http_pwm(&g_T_drone_self);
		/*g_T_drone_self.arrd_current_pwm[0] = g_T_drone_self.arrd_current_pwm[0] / 10;
		g_T_drone_self.arrd_current_pwm[1] = g_T_drone_self.arrd_current_pwm[1] / 10;
		g_T_drone_self.arrd_current_pwm[2] = g_T_drone_self.arrd_current_pwm[2] / 10;
		g_T_drone_self.arrd_current_pwm[3] = g_T_drone_self.arrd_current_pwm[3] / 10;*/

		arrd_current_duty[0] = g_T_drone_self.arrd_current_pwm[0] * 40000;
		arrd_current_duty[1] = g_T_drone_self.arrd_current_pwm[1] * 40000;
		arrd_current_duty[2] = g_T_drone_self.arrd_current_pwm[2] * 40000;
		arrd_current_duty[3] = g_T_drone_self.arrd_current_pwm[3] * 40000;


		arri_i2c_output[0] = ((int)arrd_current_duty[0]) / 256;
		arri_i2c_output[1] = ((int)arrd_current_duty[0]) % 256;
		arri_i2c_output[2] = ((int)arrd_current_duty[1]) / 256;
		arri_i2c_output[3] = ((int)arrd_current_duty[1]) % 256;
		mraa_i2c_write(pwm12, arri_i2c_output, 4); //4 bytes duty data of i2c output for pwm 1 and 2

		/* Haven't been implemented
		arri_i2c_output[0] = ((int)arrd_current_duty[2]) / 256;
		arri_i2c_output[1] = ((int)arrd_current_duty[2]) % 256;
		arri_i2c_output[2] = ((int)arrd_current_duty[3]) / 256;
		arri_i2c_output[3] = ((int)arrd_current_duty[3]) % 256;
		mraa_i2c_write(pwm34, arri_i2c_output, 4); //4 bytes duty data of i2c output for pwm 3 and 4
		*/
		usleep(50000);
		printf("pwm1 = %f\n", (g_T_drone_self.arrd_current_pwm[0]));
		printf("pwm2 = %f\n", (g_T_drone_self.arrd_current_pwm[1]));
		printf("pwm3 = %f\n", (g_T_drone_self.arrd_current_pwm[2]));
		printf("pwm4 = %f\n\n", (g_T_drone_self.arrd_current_pwm[3]));
		/*float output = mraa_pwm_read(pwm1);*/
	}
	return 0;
}
