#outline
~0 stop
~1 start
~2 control
~3 pid

###autocontrol
~201######$ auto_stopFly
~202######$ auto_startFly
~203######$ auto_learnVariables

###manual control
~2101#####$ suspend
~2102#####$ up
~2103#####$ down
~2104#####$ forward
~2105#####$ backward
~2106#####$ left
~2107#####$ right
~2108#####$ clockwiseRotate
~2109#####$ anticlockwiseRotate
~21010#####$ stop
~21011#####$ pwm manual up 0.000025
~21012#####$ pwm manual down 0.000025
~21013#####$ pwm manual up 0.002000
~21014#####$ pwm manual down 0.002000
~21015#####$ null
~21016#####$ enable pwm pid ultrasound
~21017#####$ disable pwm pid ultrasound

###pid tuning
~301xxxxxxx$ kp_pitch
~302xxxxxxx$ ki_pitch
~303xxxxxxx$ kd_pitch
~304xxxxxxx$ kp_roll
~305xxxxxxx$ ki_roll
~306xxxxxxx$ kd_roll
~307xxxxxxx$ kp_yaw
~308xxxxxxx$ ki_yaw
~309xxxxxxx$ kd_yaw
~310xxxxxxx$ kp_second_pitch
~311xxxxxxx$ kd_second_pitch
~312xxxxxxx$ kp_second_roll
~313xxxxxxx$ kd_second_roll
~314xxxxxxx$ kp_second_yaw
~315xxxxxxx$ kd_second_yaw
