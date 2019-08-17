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
import os
import cv2
from vanbot_motion_detector import VanBotMotionDetector
from vanbot_settings import VanBotSettings


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
    def __init__(self):
        #last alarm state
        self.alarm = False
        self.notify_alarm = True
        self.md = VanBotMotionDetector('inside')
        self.bot = Bot()
        self.bot.update_listener(self.new_message)
        self.lastFilename = '/home/beni/workspace/solar/vanbot/output.txt'

    def on_alarm(self, alarm, name):
        if self.notify_alarm:
            if alarm:
                self.bot.send_text("Alarm from camera '{0}'".format(name))
                self.send_last_frame()
            else:
                self.bot.send_text("Alarm gone from camera {0}".format(name))


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
                    self.md.alarmTimeSeconds = t
                    message.reply_text("Alarm time set: {0}".format(t))
                except:
                    message.reply_text("Command error")

            elif msg == 'alarm on':
                self.notify_alarm = True
                message.reply_text("Alarm is On")

            elif msg == 'alarm off':
                self.notify_alarm = False
                message.reply_text("Alarm is Off")

            elif msg == 'rec start':
                filename = self.md.start_recording()
                if filename is not None:
                    self.lastFilename = filename
                    message.reply_text("Recording started: {0}".format(filename))
                else:
                    message.reply_text("Start recording FAILED")

            elif msg == 'rec stop':
                filename = self.md.stop_recording()
                if filename is not None:
                    self.lastFilename = filename
                    message.reply_text("Recording stopped: {0}".format(filename))
                else:
                    message.reply_text("Stop recording FAILED")

            elif msg == 'rec send':
                if self.lastFilename is not None:
                    print("Sending file: {0}".format(self.lastFilename))
                    message.reply_document(open(self.lastFilename,'rb'), filename=self.lastFilename)

            else:
                message.reply_text("Unknown command")

    def send_last_frame(self):
            filename = self.md.save_picture()
            if filename is not None:
                self.bot.send_picture(filename)

    def start(self):
        self.md.start()
        self.bot.start()
        while True:
            changed = self.md.check_alarm_changed()
            if changed is not None:
                self.on_alarm(changed, self.md.name)

            sleep(0.1)  
        self.md.join()
        self.bot.join()

if __name__ == "__main__":
    alarmSystem = AlarmSystem()
    alarmSystem.start()


