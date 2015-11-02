import cv2.cv as cv
capture = cv.CaptureFromCAM(0)
img = cv.QueryFrame(capture)
cv.SaveImage("camera.jpg", img)
