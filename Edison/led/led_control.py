#!/usr/bin/env python
import mraa
import time
import requests
import json
import yaml
import threading

x = mraa.Gpio(13)
x.dir(mraa.DIR_OUT)
global led_status
led_status = 0

def http_request():
    try:
        while True:
            #results_raw = requests.get("http://fryer.ee.ucla.edu/rest/api/led/get/")
            #results_json = results_raw.text
            #results = yaml.safe_load(results_json)
            #led_status = int(results['data']['led_status'])
            print 'thread %s is running...' % threading.current_thread().name
    except KeyboardInterrupt:
            print 'parent received ctrl-c'


http_request_thread = threading.Thread(target=http_request, name='httpRequestThread')
http_request_thread.start()
http_request_thread.join()

while True:
    x.write(led_status)
