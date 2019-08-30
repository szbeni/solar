import time
from threading import Thread
from flask import Flask, render_template, Response
import cv2


class VanBotHTTPStreamer(Thread):
    def __init__(self, settings):
        super().__init__()
        self.running = False
        self.settings = settings

        self.app = Flask("Vanbot", template_folder=self.settings['template_folder'])
        self.app.add_url_rule("/", view_func=self.index)
        self.app.add_url_rule("/video_feed", view_func=self.video_feed)
        self.new_frame = False
        self.frame = None

    def on_new_frame(self, frame):
        self.new_frame = True
        self.frame = frame

    def index(self):
        timeNow = time.asctime( time.localtime(time.time()) )
        templateData = {
            'time': timeNow
        }
        return render_template('index.html', **templateData)

    def video_feed(self):
        #Video streaming route. Put this in the src attribute of an img tag
        return Response(self.gen(), mimetype='multipart/x-mixed-replace; boundary=frame')

    def run(self):
        self.running = True
        while self.running:
            self.app.run(host=self.settings['host'], port=self.settings['port'], debug=False, threaded=True)

    def stop(self):
        self.running = False

    def gen(self):
        while True:
            if self.new_frame:
                self.new_frame = False
                img_str = cv2.imencode('.jpg', self.frame)[1].tostring()        
                yield (b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' + img_str + b'\r\n')
            time.sleep(0.02)

if __name__ == '__main__':
    from vanbot_settings import VanBotSettings
    streamer = VanBotHTTPStreamer(VanBotSettings.http_streamer)
    streamer.start()
    streamer.join()
