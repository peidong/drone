#control part

1. HTTP GET

        http://fryer.ee.ucla.edu/rest/api/control/get/?mac_address=fc:c2:de:3d:7f:af

2. HTTP POST

    Post data example

        control_type=2&auto_control_command=3&manual_control_command=5&suspend_pwm1=0.12&suspend_pwm2=0.23&suspend_pwm3=0.34&suspend_pwm4=0.45

    Url

        http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af

###Database part

Database: edison, table: control

#####mac_address

#####control_type

- 1: automatic
- 2: manual

#####auto_control_command

- 1: stopFly
- 2: startFly
- 3: learnVariables

#####manual_control_command

- 1: suspend
- 2: up
- 3: down
- 4: forward
- 5: backward
- 6: left
- 7: right
- 8: clockwiseRotate
- 9: anticlockwiseRotate
- 10: stop
- 11: pwm manual up 0.000025
- 12: pwm manual down 0.000025
- 13: pwm manual up 0.002000
- 14: pwm manual down 0.002000
- 15: null
- 16: enable pwm pid ultrasound
- 17: disable pwm pid ultrasound
- 18: start sphere fit
- 19: stop sphere fit

#####suspend_pwm1

#####suspend_pwm2

#####suspend_pwm3

#####suspend_pwm4
