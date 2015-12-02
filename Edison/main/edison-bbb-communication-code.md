#From Edison to BeagleBone Black
~0$ stop

~1$ start

###autocontrol
~201$ auto_stopFly

~202$ auto_startFly

~203$ auto_learnVariables

###manual control
~21001$ suspend

~21002$ up

~21003$ down

~21004$ forward

~21005$ backward

~21006$ left

~21007$ right

~21008$ clockwiseRotate

~21009$ anticlockwiseRotate

~21010$ stop

~21011$ pwm manual up 0.000025

~21012$ pwm manual down 0.000025

~21013$ pwm manual up 0.002000

~21014$ pwm manual down 0.002000

~21015$ null

~21016$ enable pwm pid ultrasound

~21017$ disable pwm pid ultrasound

###pid tuning (Note: x is number)
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

###feedback:

~40s(f)$ success(fail) yaw xxx (0~360)

~41s(f)$ success(fail) positive(negative)xxx.xxxxxx latitude

~42s(f)$ success(fail) positive(negative)xxx.xxxxxx longitude


#From BeagleBone Black to Edison

###GPS

~40xxx$ yaw xxx (0~360)

~41p(n)xxxxxxxxx$ positive(negative)xxx.xxxxxx latitude

~42p(n)xxxxxxxxx$ positive(negative)xxx.xxxxxx longitude

###feedback:


~0s(f)$ success(fail) stop

~1s(f)$ success(fail) start

#####autocontrol
~201s(f)$ success(fail) auto_stopFly

~202s(f)$ success(fail) auto_startFly

~203s(f)$ success(fail) auto_learnVariables

#####manual control
~21001s(f)$ success(fail) suspend

~21002s(f)$ success(fail) up

~21003s(f)$ success(fail) down

~21004s(f)$ success(fail) forward

~21005s(f)$ success(fail) backward

~21006s(f)$ success(fail) left

~21007s(f)$ success(fail) right

~21008s(f)$ success(fail) clockwiseRotate

~21009s(f)$ success(fail) anticlockwiseRotate

~21010s(f)$ success(fail) stop

~21011s(f)$ success(fail) pwm manual up 0.000025

~21012s(f)$ success(fail) pwm manual down 0.000025

~21013s(f)$ success(fail) pwm manual up 0.002000

~21014s(f)$ success(fail) pwm manual down 0.002000

~21015s(f)$ success(fail) null

~21016s(f)$ success(fail) enable pwm pid ultrasound

~21017s(f)$ success(fail) disable pwm pid ultrasound

#####pid tuning (Note: x is number)
~301s(f)$ success(fail) kp_pitch

~302s(f)$ success(fail) ki_pitch

~303s(f)$ success(fail) kd_pitch

~304s(f)$ success(fail) kp_roll

~305s(f)$ success(fail) ki_roll

~306s(f)$ success(fail) kd_roll

~307s(f)$ success(fail) kp_yaw

~308s(f)$ success(fail) ki_yaw

~309s(f)$ success(fail) kd_yaw

~310s(f)$ success(fail) kp_second_pitch

~311s(f)$ success(fail) kd_second_pitch

~312s(f)$ success(fail) kp_second_roll

~313s(f)$ success(fail) kd_second_roll

~314s(f)$ success(fail) kp_second_yaw

~315s(f)$ success(fail) kd_second_yaw
