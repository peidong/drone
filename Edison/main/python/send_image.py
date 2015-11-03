#!/usr/bin/env python
import cv2.cv as cv#opencv for image capture
import datetime
import json
import yaml#pip install pyyaml
import threading
import requests#sudo pip install requests

def get_str_filename():
    str_filename = str(datetime.datetime.now())
    str_filename = str_filename.replace(' ','-')
    str_filename = "files/" + str_filename + ".jpg"
    return str_filename

def CaptureImage(str_filename):
    capture = cv.CaptureFromCAM(0)
    img = cv.QueryFrame(capture)
    cv.SaveImage(str_filename, img)

def UploadFile(str_filename, str_url):
    files = {'file': open(str_filename)}
    response = requests.post(str_url, files=files)

def get_json_HTTP(str_url):
    results_raw = requests.get(str_url)
    results_json = results_raw.text
    results = yaml.safe_load(results_json)
    return results

#main function
str_filename = get_str_filename()
CaptureImage(str_filename)
UploadFile(str_filename, "http://fryer.ee.ucla.edu/rest/api/upload/")
print(get_json_HTTP("http://fryer.ee.ucla.edu/rest/api/camera/get/"))
