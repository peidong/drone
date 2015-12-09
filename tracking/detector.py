import numpy as np
import cv2
import sys
import time
import requests
from PIL import Image
from StringIO import StringIO
# import mraa
roiPts = []
inputMode = False
class App(object):
    def __init__(self,cap,selection):
        self.cam = cap
        ret, self.frame = self.cam.read()
        cv2.namedWindow('camshift')
        
        self.origin_x = 0
        self.origin_y = 0
        self.origin_area = 0
        self.selection = selection
        self.drag_start = None
        self.tracking_state = 1
        self.show_backproj = False

    def run(self):
        number=1
        formerCommand = -1;
        while True:
            ret, self.frame = self.cam.read()
            vis = self.frame.copy()
            hsv = cv2.cvtColor(self.frame, cv2.COLOR_BGR2HSV)
            mask = cv2.inRange(hsv, np.array((0., 60., 32.)), np.array((180., 255., 255.)))

            if self.selection:
                x0, y0, x1, y1 = self.selection
                self.track_window = (x0, y0, x1-x0, y1-y0)
                hsv_roi = hsv[y0:y1, x0:x1]
                mask_roi = mask[y0:y1, x0:x1]
                hist = cv2.calcHist( [hsv_roi], [0], mask_roi, [16], [0, 180] )
                cv2.normalize(hist, hist, 0, 255, cv2.NORM_MINMAX)
                self.hist = hist.reshape(-1)

                vis_roi = vis[y0:y1, x0:x1]
                cv2.bitwise_not(vis_roi, vis_roi)
                vis[mask == 0] = 0
             
            if self.tracking_state == 1:
                self.selection = None
                prob = cv2.calcBackProject([hsv], [0], self.hist, [0, 180], 1)
                prob &= mask
                term_crit = ( cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1 )
                # if prob:
                track_box, self.track_window = cv2.CamShift(prob, self.track_window, term_crit)
                # print self.track_window
                if number>0 and number<15:
                    number=number+1
                    continue
                if number==15:
                    self.origin_area=self.track_window[2]*self.track_window[3]
                    self.origin_x=self.track_window[0]
                    self.origin_y=self.track_window[1]
                    print "start tracking"
                    number=-1
                if self.track_window[0]<2 or self.track_window[1]<2 or (self.track_window[0]+self.track_window[2])>646 or (self.track_window[1]+self.track_window[3]>472):
                    print "lost detection"
                    break
                a=(self.track_window[2])*(self.track_window[3])
                current_x=self.track_window[0]
                current_y=self.track_window[1]
                # print "orgin location is %f,%f,%f"%(self.origin_x,self.origin_y,self.origin_area)
                # print "current location is %f,%f"%(current_x,current_y)
                # print self.track_window
                x_difference = current_x - self.origin_x
                area_difference = a - self.origin_area
                if x_difference < -25:
                    print "turn right"
                    if formerCommand !=8:
                        r = requests.post("http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af",data = {"manual_control_command":"8"})
                        formerCommand = 8
                elif x_difference > 25:
                    print "turn left"
                    if formerCommand !=9:
                        r = requests.post("http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af",data = {"manual_control_command":"9"})
                        formerCommand = 9
                else:
                    if area_difference < -2000:
                        print "move forward"
                        if formerCommand !=4:
                            r = requests.post("http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af",data = {"manual_control_command":"4"})
                            formerCommand = 4
                    elif area_difference > 2000:
                        print "move backward"
                        if formerCommand !=5:
                            r = requests.post("http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af",data = {"manual_control_command":"5"})
                            formerCommand = 5
                    else:
                        print "don't move"
                        if formerCommand !=1:
                            r = requests.post("http://fryer.ee.ucla.edu/rest/api/control/post/?mac_address=fc:c2:de:3d:7f:af",data = {"manual_control_command":"1"})
                            formerCommand = 1
                if self.show_backproj:
                    vis[:] = prob[...,np.newaxis]
                try:
                    cv2.ellipse(vis, track_box, (0, 0, 255), 2)
                except:
                    print(track_box)

                # if number == 20:
                    # SI.get_flag_value("http://128.97.89.181/rest/api/camera/get/")
                    # str_filename = SI.get_str_filename()
                    # str_filename = vis
                    # SI.UploadFile(str_filename, "http://128.97.89.181/rest/api/upload/")
                    # print("upload file success")
                    # number = -1

            cv2.imshow('camshift', vis)
            ch = 0xFF & cv2.waitKey(5)
            if ch == 27:
                break
            if ch == ord('b'):
                self.show_backproj = not self.show_backproj
        cv2.destroyAllWindows()

# def uart_send(string):
    # u = mraa.Uart(0)
    # setBaudRate(u, baud)
    # writeStr(u,string)

def detection(img1, img2):
    sift = cv2.SIFT()
    kp1, des1 = sift.detectAndCompute(img1,None)
    kp2, des2 = sift.detectAndCompute(img2,None)
    while(des2==None):
        print "no matches"
        rst,img2_GDB = cap.read()
        img2=cv2.cvtColor(img2_GDB,cv2.COLOR_BGR2GRAY)
        kp2,des2=sift.detectAndCompute(img2,None)
    # k=cv2.waitKey(0)
    FLANN_INDEX_KDTREE = 0
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
    search_params = dict(checks = 50)
    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(des1,des2,k=2)
    good = []
    for m,n in matches:
        if m.distance < 0.7*n.distance:
            good.append(m)

    if len(good)>MIN_MATCH_COUNT:
        src_pts = np.float32([ kp1[m.queryIdx].pt for m in good ]).reshape(-1,1,2)
        dst_pts = np.float32([ kp2[m.trainIdx].pt for m in good ]).reshape(-1,1,2)
        M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC,5.0)
        matchesMask = mask.ravel().tolist()
        h,w = img1.shape[:2]
        pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
        dst = cv2.perspectiveTransform(pts,M)
        rows,cols = img1.shape[:2]
        origin_corner = np.float32([[0,0], [cols,0],[cols,rows],[0,rows]]).reshape(-1,1,2)
        corner = cv2.perspectiveTransform(origin_corner,M)
        # print corner
        y_axis=[corner[0][0][1], corner[1][0][1], corner[2][0][1], corner[3][0][1]]
        x_axis =[corner[0][0][0], corner[1][0][0], corner[2][0][0], corner[3][0][0]]
        x_max= max(x_axis)
        y_max= max(y_axis)
        x_min= min(x_axis)
        y_min= min(y_axis)
        w=x_max-x_min
        h=y_max-y_min
        bb=(x_min,y_min,x_max,y_max)
        area=w*h
        if area<100 or x_min <0 or y_min<0 or x_max<x_min or y_max<y_min:
            print "found error"
            return (-1,-1,-1,-1),-1
        return bb,area
    else:
        print "Not enough matches are found - %d/%d" % (len(good),MIN_MATCH_COUNT)
        matchesMask = None
        return (-1,-1,-1,-1),-1

def DownloadFile(str_url):
    response = requests.get(str_url)
    img = np.array(Image.open(StringIO(response.content)))
    return img

# def start():
if __name__ == '__main__':
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
    # start()
