#!/usr/bin/python3

# -*- coding: utf-8 -*-
"""
python-telegram-bot
imutils

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
from motion_detector import MotionDetector
import os
import cv2

class Bot(Thread):
    def __init__(self, token):
        super().__init__()
        # Telegram Bot Authorization Token
        self.bot = telegram.Bot(token)

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
        print ("Starting telebot")
        while True:
            try:
                for update in self.bot.get_updates(offset=self.update_id, timeout=10):
                    self.update_id = update.update_id + 1

                    if update.message:  # your bot can receive updates without messages
                        if self.listener is not None:
                            self.listener(update.message)
                        # Reply to the message
                        #update.message.reply_text(update.message.text)
                        #filename = "/home/beni/motion/test.jpeg"
                        #update.message.reply_photo(open(filename, 'rb'))
            except NetworkError:
                sleep(1)
            except Unauthorized:
                # The user has removed or blocked the bot.
                self.update_id += 1

        print ("Exiting telebot")


    def send_picture(self, filename):
        try:
            f = open(filename, 'rb')
            self.bot.send_photo(953799736, f)
        except:
            print("Error sending photo: ", filename)
        print("Picture send:", filename)

class AlarmSystem:
    def __init__(self):
        #last alarm state
        self.alarm = False
        self.notify_alarm = True
        self.md = MotionDetector(0,30)
        self.bot = Bot("885426852:AAGw6a6bituVbltm0VFA1Z3PW0TFhzK_AIE")
        self.bot.update_listener(self.new_message)

    def new_message(self, message):
        if message.from_user.username == 'szbeni':
            msg = message.text.lower()
            if msg == 'get':
                print("Get last frame command")
                self.send_last_frame()
            elif msg.startswith('set alarm time'):
                try:
                    t = int(msg.replace('set alarm time',''))
                    self.md.alarmTimeSeconds = t
                    message.reply_text("Alarm time set: " + str(t))
                except:
                    message.reply_text("Command error")

            elif msg == 'alarm on':
                self.notify_alarm = True
                message.reply_text("Alarm is On")

            elif msg == 'alarm off':
                self.notify_alarm = False
                message.reply_text("Alarm is Off")

            else:
                message.reply_text("Unknown command")

    def send_last_frame(self):
            filename = '/tmp/motion.png'
            frame = self.md.getLastFrame()
            if frame is not None:
                cv2.imwrite(filename, frame)    
                self.bot.send_picture(filename)

    def start(self):
        self.md.start()
        self.bot.start()
        print("started")

        while True:
            alarm = self.md.alarm
            if self.alarm != alarm:
                self.alarm = alarm
                if self.notify_alarm:
                    if alarm:
                        print("Alarm!")
                        self.send_last_frame()
                    else:
                        print("Alarm gone..")
                        
            sleep(0.1)

        self.md.join()
        self.bot.join()

if __name__ == "__main__":
    alarmSystem = AlarmSystem()
    alarmSystem.start()


