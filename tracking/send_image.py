#!/usr/bin/env python
import cv2.cv as cv#opencv for image capture
import datetime
import time
import json
import yaml#pip install pyyaml
import threading
import requests#sudo pip install requests
from PIL import Image
# from io import BytesIO
import numpy as np
from StringIO import StringIO

global model_variable
# global n_image_flag
# global n_video_flag
# n_image_flag = 0
# n_video_flag = 0

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

def get_flag_value(str_url):
    # global n_image_flag
    global model_variable
    results = get_json_HTTP(str_url)
    model_variable = int(results['data']['video'])
    # n_image_flag = int(results['data']['image'])
    # n_video_flag = int(results['data']['video'])

def DownloadFile(str_url):
    response = requests.get(str_url)
    img = np.array(Image.open(StringIO(response.content)))
    return img

#main function
# def main():
if __name__ == '__main__':
    global model_variable
    # global n_video_flag
    while 1:
        get_flag_value("http://fryer.ee.ucla.edu/rest/api/camera/get/")
        time.sleep(1)
        # print("get flag value success")
        # if n_image_flag == 0:
            # continue
        str_filename = get_str_filename()
        CaptureImage(str_filename)
        UploadFile(str_filename, "http://fryer.ee.ucla.edu/rest/api/upload/")
        print("upload file success")
        if model_variable == 1:
            MIN_MATCH_COUNT = 5
            cap = cv2.VideoCapture(1)
            # img1 = cv2.imread('reference.jpg',1)          # queryImage
            img1_GDB = DownloadFile("http://fryer.ee.ucla.edu/rest/api/upload/files/reference1.jpg")
            img1=cv2.cvtColor(img1_GDB,cv2.COLOR_BGR2GRAY)
            while True:
                ret,img2_GDB = cap.read()
                img2=cv2.cvtColor(img2_GDB,cv2.COLOR_BGR2GRAY)
                ret,img3 = cap.read()
                bb,area = detection(img1,img2)
                cv2.rectangle(img2_GDB, (bb[0], bb[1]), (bb[2],bb[3]), (255,0,0), 2)
                cv2.imshow("lalala", img2_GDB)
 
                video_src=2
                if area != -1:
                        print "detected"
                        selection=bb
                        App(cap,selection).run()
                        ch = cv2.waitKey(5)
                if 0xFF&ch == 27:
                        break
            cv2.destroyAllWindows()        
# if __name__ == '__main__':
# main()
