#From Edison to BeagleBone Black
~000$ stop

###autocontrol
~110$ auto_stopFly

~120$ auto_startFly

~130$ auto_learnVariables

###manual control
~201$ suspend

~202$ up

~203$ down

~204$ forward

~205$ backward

~206$ left

~207$ right

~208$ clockwiseRotate

~209$ anticlockwiseRotate

~210$ stop

~211$ pwm manual up 0.000025

~212$ pwm manual down 0.000025

~213$ pwm manual up 0.002000

~214$ pwm manual down 0.002000

~215$ null

~216$ enable pwm pid ultrasound

~217$ disable pwm pid ultrasound

~218$ start sphere fit

~219$ stop sphere fit

###pid tuning (Note: x is number)
~301x.xxxxxx$ kp_pitch

~302x.xxxxxx$ ki_pitch

~303x.xxxxxx$ kd_pitch

~304x.xxxxxx$ kp_roll

~305x.xxxxxx$ ki_roll

~306x.xxxxxx$ kd_roll

~307x.xxxxxx$ kp_yaw

~308x.xxxxxx$ ki_yaw

~309x.xxxxxx$ kd_yaw

~310x.xxxxxx$ kp_second_pitch

~311x.xxxxxx$ kd_second_pitch

~312x.xxxxxx$ kp_second_roll

~313x.xxxxxx$ kd_second_roll

~314x.xxxxxx$ kp_second_yaw

~315x.xxxxxx$ kd_second_yaw

###feedback:

~40s(f)$ success(fail) yaw xxx (0~360)

~41s(f)$ success(fail) positive(negative)xxx.xxxxxx latitude

~42s(f)$ success(fail) positive(negative)xxx.xxxxxx longitude


#From BeagleBone Black to Edison

###GPS

~4xxx|-xxx.xxxxxx|-xxx.xxxxxx$ face_direction latitude longitude not fix length

<!--~40xxx$ yaw xxx (0~360)-->

<!--~41p(n)xxx.xxxxxx$ positive(negative)xxx.xxxxxx latitude-->

<!--~42p(n)xxx.xxxxxx$ positive(negative)xxx.xxxxxx longitude-->

###feedback:

~0s(f)$ success(fail) stop

#####autocontrol
~11s(f)$ success(fail) auto_stopFly

~12s(f)$ success(fail) auto_startFly

~13s(f)$ success(fail) auto_learnVariables

#####manual control
~201s(f)$ success(fail) suspend

~202s(f)$ success(fail) up

~203s(f)$ success(fail) down

~204s(f)$ success(fail) forward

~205s(f)$ success(fail) backward

~206s(f)$ success(fail) left

~207s(f)$ success(fail) right

~208s(f)$ success(fail) clockwiseRotate

~209s(f)$ success(fail) anticlockwiseRotate

~210s(f)$ success(fail) stop

~211s(f)$ success(fail) pwm manual up 0.000025

~212s(f)$ success(fail) pwm manual down 0.000025

~213s(f)$ success(fail) pwm manual up 0.002000

~214s(f)$ success(fail) pwm manual down 0.002000

~215s(f)$ success(fail) null

~216s(f)$ success(fail) enable pwm pid ultrasound

~217s(f)$ success(fail) disable pwm pid ultrasound

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
