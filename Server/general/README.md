#General info update part
1. To get your edison board general value

        http://fryer.ee.ucla.edu/rest/api/general/get/
2. Update the pwm value

        curl --data "pwm1=0.12&pwm2=0.23&pwm3=0.34&pwm4=0.45" "http://fryer.ee.ucla.edu/rest/api/general/post/"
