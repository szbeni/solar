import cv2
import numpy as numpy
from imutils.video import VideoStream
import imutils
import datetime
from time import time, sleep, strftime
from threading import Thread
from vanbot_settings import VanBotSettings

class VanBotMotionDetector(Thread):
    def __init__(self, name, display=False):
        super().__init__()
        self.running = False

        self.display = display
        self.name = name
        self.settings = VanBotSettings.webcam['inside']
        self.motionStarted = 0
        self.motionStopped = 0
        self.motion =  False
        self.motionPrev = False
        self.frame_prev = None
        self.on_new_frame_callback = None

        self.alarm_enabled = False
        self.alarm = False
        self.alarm_last_check = False
        self.alarmTimeSeconds = self.settings['alarm_time_seconds']
        self.alarmMinDuration = self.settings['min_alarm_duration']
        self.minMotionArea = self.settings['min_motion_area']


        self.cap = None
        self.fgbg = cv2.createBackgroundSubtractorMOG2()
        self.lastFrame = None
        self.videoRecorder = None

    def register_new_frame_callback(self, f):
        self.on_new_frame_callback = f

    def alarm_on(self):
        self.alarm_enabled = True

    def alarm_off(self):
        self.alarm_enabled = False


    def draw_contours(self, frame, contours, mask_flag = False):
        if mask_flag == True:
            mask = numpy.zeros(frame.shape, dtype = "uint8")

        middle_points = []
        # loop over the contours
        for c in contours:

            # if the contour is too small, ignore it
            if cv2.contourArea(c) < self.minMotionArea:
                    continue

            # contour data
            M = cv2.moments(c)
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            x, y, w, h = cv2.boundingRect(c)
            rx = x+int(w/2)
            ry = y+int(h/2)
            ca = cv2.contourArea(c)

            # plot contours
            cv2.drawContours(frame, [c], 0, (0, 0, 255), 2)
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            if mask_flag:
                cv2.rectangle(mask, (x, y), (x+w, y+h), (255, 255, 255), -1)

            cv2.circle(frame, (cx, cy), 2, (0, 0, 255), 2)
            cv2.circle(frame, (rx, ry), 2, (0, 255, 0), 2)

            middle_points.append((rx, ry, ca))
        if mask_flag:
            return mask, middle_points
        else:
            return frame, middle_points


    def newFrame(self, frame):
        visualise = frame.copy()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        if self.frame_prev is None:
            self.frame_prev = gray.copy()
            return

        gray_blurred = cv2.GaussianBlur(gray, (21, 21), 0)


        # delta frame
        frame_delta = cv2.absdiff(self.frame_prev, gray_blurred)
        mask_motion = cv2.threshold(frame_delta, 15, 255, cv2.THRESH_BINARY)[1]

        # dilate the thresholded image to fill in holes
        kernel = numpy.ones((5, 5), numpy.uint8)
        mask_motion = cv2.dilate(mask_motion, kernel, iterations=4)


        #fgmask = self.fgbg.apply(gray_blurred)
        #thresh = cv2.threshold(fgmask, 25, 255, cv2.THRESH_BINARY)[1]
        #thresh = cv2.dilate(thresh, None, iterations=2)

		#nada, contours_motion, nada = cv2.findContours(mask_motion, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cv2.findContours(mask_motion.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        #cnts = imutils.grab_contours(cnts)

        
        contourFound = False
        for c in cnts[1]:
            if cv2.contourArea(c) < self.minMotionArea:
                continue
            (x, y, w, h) = cv2.boundingRect(c)
            cv2.rectangle(visualise, (x, y), (x + w, y + h), (0, 255, 0), 2)
            contourFound = True
            break
            
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

        if self.alarm_enabled == False:
            alarm = False

        if self.alarm != alarm:
            self.alarm = alarm


        cv2.putText(visualise, datetime.datetime.now().strftime("%A %d %B %Y %I:%M:%S%p"),(10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(self.motionTime),(10, frame.shape[0] - 30), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(self.alarm),(10, frame.shape[0] - 50), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        cv2.putText(visualise, str(timeSinceMotionStopped),(10, frame.shape[0] - 70), cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)

        if self.display:
            cv2.imshow('original', visualise)
            cv2.imshow('gray', gray)
            #cv2.imshow('thresh', thresh)
            cv2.imshow('mask_motion', mask_motion)
            (mask_motion_countours, middle_points) = self.draw_contours(visualise, cnts[1], False)
            cv2.imshow('mask_motion_countours', mask_motion_countours)

            

        cv2.waitKey(30)
        self.lastFrame = visualise
        self.frame_prev = gray_blurred.copy()

        if self.on_new_frame_callback is not None:
            self.on_new_frame_callback(self.lastFrame)

    def openStream(self):
        print("Open Stream: ", self.settings['webcam_source'])
        if self.cap is not None:
            self.cap.release()
            self.cap = None

        self.cap = cv2.VideoCapture(self.settings['webcam_source'])
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
    md = VanBotMotionDetector('inside', display=True)
    md.start()
    #md.start_recording()
    #sleep(3)
    #md.stop_recording()
    #sleep(1)
    #md.stop()
    md.join()
    pass
