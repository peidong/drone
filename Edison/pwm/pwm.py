#!/usr/bin/env python
import mraa
import time
import requests
import json
import yaml
import threading

pwm1 = mraa.Pwm(3)
# pwm2 = mraa.Pwm(5)
# pwm3 = mraa.Pwm(6)
# pwm4 = mraa.Pwm(9)

pwm1.period_us(20000)
# pwm2.period_us(20000)
# pwm3.period_us(20000)
# pwm4.period_us(20000)

pwm1.enable(True)
# pwm2.enable(True)
# pwm3.enable(True)
# pwm4.enable(True)

while True:

    results_raw = requests.get("http://fryer.ee.ucla.edu/rest/api/pwm/get/")
    results_json = results_raw.text
    results = yaml.safe_load(results_json)
    pwm1_value = float(results['data']['pwm1'])
    # pwm2_value = float(results['data']['pwm2'])
    # pwm3_value = float(results['data']['pwm3'])
    # pwm4_value = float(results['data']['pwm4'])
    print "pwm1 = " + str(pwm1_value)
    # print "pwm2 = " + str(pwm2_value)
    # print "pwm3 = " + str(pwm3_value)
    # print "pwm4 = " + str(pwm4_value)

    pwm1.write(pwm1_value)
    time.sleep(0.05)
