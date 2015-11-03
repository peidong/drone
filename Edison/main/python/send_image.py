#!/usr/bin/env python
import cv2.cv as cv#opencv for image capture
import datetime
import json
import yaml
import threading

def capture_image(str_filename):
    capture = cv.CaptureFromCAM(0)
    img = cv.QueryFrame(capture)
    cv.SaveImage("files/"+str_filename+".jpg", img)

# capture_image(str(datetime.datetime.now()))
# print datetime.datetime.now()


