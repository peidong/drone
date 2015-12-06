#include <mraa.h>
#include <stdio.h>
#include <string.h>

#define stopall					"000"

#define auto_stopFly 			"101"
#define auto_startFly 			"102"
#define auto_learnVariables 	"103"

#define suspend		 			"201"
#define up 						"202"
#define down		 			"203"
#define forward		 			"204"
#define backward	 			"205"
#define left 					"206"
#define right		 			"207"
#define clockwiseRotate			"208"
#define anticlockwiseRotate		"209"
#define stop					"210"
#define pwmup_micro	 			"211"
#define pwmdown_micro	 		"212"
#define pwmup_macro	 			"213"
#define pwmdown_macro	 		"214"
#define nc						"215"
#define en_pwmpid		 		"216"
#define dis_pwmpid		 		"217"

#define kp_pitch				"301"
#define ki_pitch				"302"
#define kd_pitch				"303"
#define kp_roll					"304"
#define ki_roll					"305"
#define kd_roll					"306"
#define kp_yaw					"307"
#define ki_yaw					"308"
#define kd_yaw					"309"
#define kp_2_pitch				"310"
#define kd_2_pitch				"311"
#define kp_2_roll				"312"
#define kd_2_roll				"313"
#define kp_2_yaw				"314"
#define kd_2_yaw				"315"

mraa_uart_context ed;

void send_instr(const char* ins)
{
	char mess[5];
	sprintf(mess, "~%s$", ins);
	mraa_uart_write(ed, mess, 5);
}

void send_test_pid(const char* ins, float val)
{

}

int main(){	
	
	ed = mraa_uart_init(0);
	mraa_uart_set_baudrate(ed, 38400);
	char buf[13] = "~Hello,there!";
	char flag[1];
	int i = 0;
	while(1)
    {

    	send_instr(stopall);
    	usleep(1000);
    	send_instr(auto_stopFly);
    	usleep(1000);
    	send_instr(backward);
    	usleep(1000);
    	send_instr(en_pwmpid);
    	usleep(1000);
    	

    }


}