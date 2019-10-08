#!/usr/bin/python3

# -*- coding: utf-8 -*-
"""
python-telegram-bot
imutils
pysftp
opencv-python


Simple Bot to reply to Telegram messages.
This is built on the API wrapper, see echobot2.py to see the same example built
on the telegram.ext bot framework.
This program is dedicated to the public domain under the CC0 license.
"""
import logging
import telegram
from telegram.error import NetworkError, Unauthorized
from time import sleep
from threading import Thread
import os
import cv2
import gpsd

from vanbot_motion_detector import VanBotMotionDetector
from vanbot_settings import VanBotSettings
from vanbot_file_upload import VanBotFileUpload
from vanbot_http_streamer import VanBotHTTPStreamer


class Bot(Thread):
    def __init__(self):
        super().__init__()
        # Telegram Bot Authorization Token
        self.bot = telegram.Bot(VanBotSettings.token)

        # get the first pending update_id, this is so we can skip over it in case
        # we get an "Unauthorized" exception.
        try:
            self.update_id = self.bot.get_updates()[0].update_id
        except IndexError:
            self.update_id = None

        logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')

        self.listener = None

    def update_listener(self, listener):
        self.listener = listener

    def run(self):
        while True:
            try:
                for update in self.bot.get_updates(offset=self.update_id, timeout=10):
                    self.update_id = update.update_id + 1

                    if update.message:  # your bot can receive updates without messages
                        if self.listener is not None:
                            self.listener(update.message)
                        # Reply to the message

            except NetworkError:
                sleep(1)
            except Unauthorized:
                # The user has removed or blocked the bot.
                self.update_id += 1


    def send_text(self, text):
        self.bot.send_message(VanBotSettings.chat_id, text)

    def send_document(self, filename):
        try:
            f = open(filename, 'rb')
            self.bot.send_document(VanBotSettings.chat_id, f)
        except:
            print("Error sending document: ", filename)
        print("Document sent:", filename)
        
    def send_picture(self, filename):
        try:
            f = open(filename, 'rb')
            self.bot.send_photo(VanBotSettings.chat_id, f)
        except:
            print("Error sending photo: ", filename)
        print("Picture sent:", filename)

class AlarmSystem:
    commamds = [
        {'name': 'get', 'description':'Get snapshot of the videos'},
        {'name': 'alarm on', 'description':'Switch alarm on'},
        {'name': 'alarm off', 'description':'Switch alarm off'},
        {'name': 'rec start', 'description':'Start video recording'},
        {'name': 'rec stop', 'description':'Stop video recording'},
        {'name': 'rec send', 'description':'Send last recorded video, doesnt work'},
        {'name': 'gps', 'description':'Get GPS coordinates'},
        {'name': 'help', 'description':'Display help'}
    ]

    def __init__(self):
        #last alarm state
        self.notify_alarm = False
        self.md_in = VanBotMotionDetector('inside')
        #self.md_out = VanBotMotionDetector('outside')

        self.md = []
        self.md.append(self.md_in)
        #self.md.append(self.md_out)

        self.streamer = VanBotHTTPStreamer(VanBotSettings.http_streamer)
        self.bot = Bot()
        self.bot.update_listener(self.new_message)
        self.lastFilename = None
        self.uploader = VanBotFileUpload(VanBotSettings.upload_settings)

    def on_alarm(self, alarm, name):
            if alarm:
                if self.notify_alarm:
                    self.alarm = True 
                    self.bot.send_text("Alarm from camera '{0}'".format(name))
                    filenames = self.send_last_frame()
                    for filename in filenames:
                        if filename:
                            self.uploader.upload(filename)
                    
                    for md in self.md:
                        md.start_recording()
                    
            else:
                if self.alarm:
                    self.alarm = False
                    self.bot.send_text("Alarm gone from camera {0}".format(name))
                    for md in self.md:
                        filename = md.stop_recording()
                        if filename:
                            self.uploader.upload(filename)
                        
    def start_recording(self, md, message=None):
        filename = md.start_recording()
        if filename is not None:
            self.lastFilename = filename
        return filename

    def stop_recording(self, md, message=None):
            filename = md.stop_recording()
            if filename is not None:
                self.lastFilename = filename
            return filename



    def new_message(self, message):
        if message.from_user.username == VanBotSettings.username:
            try:
                msg = message.text.lower()
            except:
                print("Not a text message")
                msg = ""

            if msg == 'get':
                self.send_last_frame()
            elif msg.startswith('set alarm time'):
                try:
                    t = int(msg.replace('set alarm time',''))
                    for md in self.md:
                        md.alarmTimeSeconds = t
                    message.reply_text("Alarm time set: {0}".format(t))
                except:
                    message.reply_text("Command error")

            elif msg == 'alarm on':
                self.notify_alarm = True
                for md in self.md:
                    md.alarm_on()
                message.reply_text("Alarm is On")

            elif msg == 'alarm off':
                self.notify_alarm = False
                for md in self.md:
                    md.alarm_off()
                message.reply_text("Alarm is Off")

            elif msg == 'rec start':
                for md in self.md:
                    filename = self.start_recording(md)
                    if filename is not None:
                        message.reply_text("Recording started: {0}".format(filename))
                    else:
                        message.reply_text("Start recording FAILED")

            elif msg == 'rec stop':
                for md in self.md:
                    filename  = self.stop_recording(md)
                    if filename is not None:
                        message.reply_text("Recording stopped: {0}".format(filename))
                    else:
                        message.reply_text("Stop recording FAILED")

            elif msg == 'rec send':
                if self.lastFilename is not None:
                    print("Sending file: {0}".format(self.lastFilename))
                    message.reply_document(open(self.lastFilename,'rb'), filename=self.lastFilename)

            elif msg == 'gps':
                try:
                    gpsd.connect()
                    packet = gpsd.get_current()
                    s = "{0}, {1}\nhttps://www.google.com/maps/search/?api=1&query={0},{1}".format(packet.position()[0],packet.position()[1])
                    message.reply_text(s)
                except:
                    message.reply_text("Cannot get gps coordinates")

            elif msg == 'help':
                s = "Available commands:\n"
                for cmd in self.commamds:
                    s += "{0} - {1}\n".format(cmd['name'], cmd['description'])
                message.reply_text(s)

            else:
                message.reply_text("Unknown command")

    def send_last_frame(self):
        filenames = []
        for md in self.md:
            filename = md.save_picture()
            if filename is not None:
                self.bot.send_picture(filename)
            filenames.append(filename)
        return filenames  


    def start(self):
        for md in self.md:
            md.start()
            #segmentation fault when trying to open socket from 2 different thread.. strange, make some testing
            sleep(3)

        self.bot.start()
        self.uploader.start()
        
        self.streamer.start()
        
        #TODO check all cameras
        self.md_in.register_new_frame_callback(self.streamer.on_new_frame)

        self.bot.send_text("VanBot started")
        while True:
            changed = self.md_in.check_alarm_changed()
            if changed is not None:
                self.on_alarm(changed, self.md_in.name)

            sleep(0.1) 
        
        self.bot.send_text("VanBot stopped")
        self.uploader.stop()
        for md in self.md:
            md.stop()

        for md in self.md:
            md.join()

        self.bot.join()
        self.streamer.join()

if __name__ == "__main__":
    alarmSystem = AlarmSystem()
    alarmSystem.start()


