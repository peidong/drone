#pid_tuning update part
1. To get your edison board pid_tuning value

        http://fryer.ee.ucla.edu/rest/api/pid_tuning/get/
2. Update the pid_tuning value

        curl --data "kp_pitch=0.0&ki_pitch=0.0&kd_pitch=0.0&kp_roll=0.0&ki_roll=0.0&kd_roll=0.0&kp_yaw=0.0&ki_yaw=0.0&kd_yaw=0.0" "http://fryer.ee.ucla.edu/rest/api/pid_tuning/post/"
