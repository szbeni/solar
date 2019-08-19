import cv2
import numpy as numpy
from imutils.video import VideoStream
import imutils
import datetime
from time import time, sleep, strftime
from threading import Thread
from vanbot_settings import VanBotSettings

class VanBotMotionDetector(Thread):
    def __init__(self, name):
        super().__init__()
        self.running = False

        self.name = name
        self.settings = VanBotSettings.webcam['inside']
        self.motionStarted = 0
        self.motionStopped = 0
        self.motion =  False
        self.motionPrev = False

        self.alarm = False
        self.alarm_last_check = False
        self.alarmTimeSeconds = self.settings['alarm_time_seconds']
        self.alarmMinDuration = self.settings['min_alarm_duration']

        self.cap = None
        self.fgbg = cv2.createBackgroundSubtractorMOG2()
        self.lastFrame = None
        self.videoRecorder = None

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

        now = time()
        #if motion has started
        if self.motion != self.motionPrev:
            self.motionPrev = self.motion
            if self.motion:
                self.motionStarted = now
                self.motionStopped = now
            else:
                self.motionStopped = now


        alarm = False
        if self.motion:
            self.motionStopped = now
            self.motionTime = time() - self.motionStarted
            if self.motionTime > self.alarmTimeSeconds:
                alarm = True
        else:
            self.motionTime = 0

        timeSinceMotionStopped = now - self.motionStopped
        if self.alarm:
            if timeSinceMotionStopped < self.alarmMinDuration:
                alarm = True

        if self.alarm != alarm:
            self.alarm = alarm


        cv2.putText(visualise, datetime.datetime.now().strftime("%A %d %B %Y %I:%M:%S%p"),(10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(self.motionTime),(10, frame.shape[0] - 30), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(self.alarm),(10, frame.shape[0] - 50), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(timeSinceMotionStopped),(10, frame.shape[0] - 70), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)

        #cv2.imshow('original', visualise)
        #cv2.imshow('gray', gray)
        #cv2.imshow('thresh', thresh)
        cv2.waitKey(30)
        self.lastFrame = visualise

    def openStream(self):
        print("Open Stream: ", self.settings['webcam_source'])
        if self.cap is not None:
            self.cap.release()
            self.cap = None

        self.cap = cv2.VideoCapture(0)
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH,self.settings['resolution'][0])
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT,self.settings['resolution'][1])
        self.cap.set(cv2.CAP_PROP_FPS,self.settings['framerate'])

    def closeStream(self):
        if self.cap is not None:
            self.cap.release()
            self.cap = None

    def start_recording(self, filename=None):
        if filename is None:
            filename = self.settings['video_filename'] + strftime("%Y%m%d_%H%M%S") + '.mp4'

        if self.videoRecorder is None:
            self.videoRecorderStop = False

            self.videoFilename = filename
            self.videoRecorder = cv2.VideoWriter(filename,cv2.VideoWriter_fourcc('m','p','4','v'), self.settings['rec_framerate'], self.settings['rec_resolution'])
            print("Started recoding {0}".format(filename))
            return filename

        return None

    def stop_recording(self):
        if self.videoRecorder is not None:
            self.videoRecorderStop = True
            print("Stopped recoding {0}".format(self.videoFilename))
            return self.videoFilename
        return None

    def save_picture(self, filename=None):
        if filename is None:
            filename = self.settings['photo_filename']  + strftime("%Y%m%d_%H%M%S") + '.png'
        if self.lastFrame is not None:
            cv2.imwrite(filename, self.lastFrame)
            return filename

        return None            

    def check_alarm_changed(self):
        if self.alarm_last_check != self.alarm:
            self.alarm_last_check = self.alarm
            return self.alarm
        return None

    def stop(self):
        self.running = False

    def run(self):
        self.running = True
        self.openStream()
        while self.running:
            ret, frame = self.cap.read()
            if ret:
                if self.videoRecorder is not None:
                    if self.videoRecorderStop:
                        self.videoRecorderStop = True
                        self.videoRecorder.release()
                        self.videoRecorder = None
                    else:
                        self.videoRecorder.write(frame)
                self.newFrame(frame)
            else:
                self.closeStream()
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
    md = VanBotMotionDetector('inside')
    md.start()
    md.start_recording()
    sleep(3)
    md.stop_recording()
    sleep(1)
    md.stop()
    md.join()
