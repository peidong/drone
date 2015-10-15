#!/usr/bin/env python
import mraa
import time
import requests
import json
import yaml
import threading

pwm1 = mraa.Pwm(6)
pwm2 = mraa.Pwm(7)
pwm3 = mraa.Pwm(8)
pwm4 = mraa.Pwm(9)

pwm1.period_us(20000)
pwm2.period_us(20000)
pwm3.period_us(20000)
pwm4.period_us(20000)

pwm1.enable(True)
pwm2.enable(True)
pwm3.enable(True)
pwm4.enable(True)

while True:

    results_raw = requests.get("http://fryer.ee.ucla.edu/rest/api/pwm/get/")
    results_json = results_raw.text
    results = yaml.safe_load(results_json)
    pwm1_value = double(results['data']['pwm1'])
    pwm2_value = double(results['data']['pwm2'])
    pwm3_value = double(results['data']['pwm3'])
    pwm4_value = double(results['data']['pwm4'])

    while value<0.1:
          pwm1.write(pwm1_value)
          pwm1_value = pwm1_value + 0.001
          time.sleep(0.005)
    while pwm1_value>0.03:
          pwm1.write(pwm1_value)
          pwm1_value = value - 0.001
          time.sleep(0.005)
