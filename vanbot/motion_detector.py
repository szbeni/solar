import cv2
import numpy as numpy
from imutils.video import VideoStream
import imutils
import datetime
from time import time, sleep
from threading import Thread
import token

class MotionDetector(Thread):
    def __init__(self, sourceNum=0, alarmTimeSeconds=10):
        super().__init__()
        self.sourceNum = sourceNum
        self.motionStarted = datetime.time()
        self.motion =  False
        self.motionPrev = False
        self.alarm = False
        self.lastFrame = None
        self.fgbg = cv2.createBackgroundSubtractorMOG2()
        self.cap = None
        self.alarmTimeSeconds = alarmTimeSeconds
        self.on_alarm_handler = None

    def update_on_alarm_handler(self, handler):
        self.on_alarm_handler = handler

    def newFrame(self, frame):
        visualise = frame.copy()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (21, 21), 0)
        fgmask = self.fgbg.apply(gray)

        thresh = cv2.threshold(fgmask, 25, 255, cv2.THRESH_BINARY)[1]
        thresh = cv2.dilate(thresh, None, iterations=2)
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        contourFound = False
        for c in cnts:
            if cv2.contourArea(c) < 1000:
                continue
            (x, y, w, h) = cv2.boundingRect(c)
            cv2.rectangle(visualise, (x, y), (x + w, y + h), (0, 255, 0), 2)
            contourFound = True
            
        if contourFound:
            self.motion = True
        else:
            self.motion = False


        if self.motion == True and self.motionPrev == False:
            self.motionStarted = time()
        
        elif self.motion == False:
            self.motionStarted = time()
        
        alarm = False
        if self.motion:
            self.motionTime = time() - self.motionStarted
            if self.motionTime > self.alarmTimeSeconds:
                alarm = True
        else:
            self.motionTime = 0
        
        if self.alarm != alarm:
            self.alarm = alarm
            if self.on_alarm_handler is not None:
                self.on_alarm_handler(self.alarm)

        self.motionPrev = self.motion
        cv2.putText(visualise, datetime.datetime.now().strftime("%A %d %B %Y %I:%M:%S%p"),(10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(self.motionTime),(10, frame.shape[0] - 30), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(self.alarm),(10, frame.shape[0] - 50), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)

        #cv2.imshow('original', visualise)
        #cv2.imshow('gray', gray)
        #cv2.imshow('thresh', thresh)
        cv2.waitKey(30)
        self.lastFrame = visualise

    def openStream(self):
        print("Open Stream: ", self.sourceNum)
        if self.cap is not None:
            self.cap.stop()
        self.cap = VideoStream(src=self.sourceNum).start()

    def closeStream(self):
        if self.cap is not None:
            self.cap.stop()

    def run(self):
        self.openStream()
        while True:
            frame = self.cap.read()
            if frame is not None:
                self.newFrame(frame)
            else:
                self.cap.stop()
                print("Camera disconnected..retry in 5 seconds")
                sleep(5)
                self.openStream()

        self.closeStream()
        cv2.destroyAllWindows()

    def getLastFrame(self):
        frame = self.lastFrame
        self.lastFrame = None
        return frame


if __name__ == "__main__":
    md = MotionDetector()
    md.start()
    md.join()
