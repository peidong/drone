#pid_tuning update part
1. To get your edison board pid_tuning value

        http://fryer.ee.ucla.edu/rest/api/pid_tuning/get/
2. Update the pid_tuning value

        curl --data "kp_pitch=0.0&ki_pitch=0.0&kd_pitch=0.0&kp_roll=0.0&ki_roll=0.0&kd_roll=0.0&kp_yaw=0.0&ki_yaw=0.0&kd_yaw=0.0" "http://fryer.ee.ucla.edu/rest/api/pid_tuning/post/"

###The values are shown below

- kp_pitch
- ki_pitch
- kd_pitch
- kp_roll
- ki_roll
- kd_roll
- kp_yaw
- ki_yaw
- kd_yaw
- kp_second_pitch
- kd_second_pitch
- kp_second_roll
- kd_second_roll
- kp_second_yaw
- kd_second_yaw
