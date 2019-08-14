#!/usr/bin/python3

# -*- coding: utf-8 -*-
"""
python-telegram-bot

Simple Bot to reply to Telegram messages.
This is built on the API wrapper, see echobot2.py to see the same example built
on the telegram.ext bot framework.
This program is dedicated to the public domain under the CC0 license.
"""
import logging
import telegram
from telegram.error import NetworkError, Unauthorized
from time import sleep
import os
from threading import Thread


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

    def run(self):
        print ("Starting telebot")
        while True:
            try:
                for update in self.bot.get_updates(offset=self.update_id, timeout=10):
                    self.update_id = update.update_id + 1

                    if update.message:  # your bot can receive updates without messages
                        # Reply to the message
                        update.message.reply_text(update.message.text)
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


bot = Bot("885426852:AAGw6a6bituVbltm0VFA1Z3PW0TFhzK_AIE")
bot.start()

#watch filesystem:
path_to_watch = "/home/pi/motion"
before = dict ([(f, None) for f in os.listdir (path_to_watch)])
while True:
  sleep(1)
  after = dict ([(f, None) for f in os.listdir (path_to_watch)])
  added = [f for f in after if not f in before]
  removed = [f for f in before if not f in after]
  if added:
    for a in added:
        filename = os.path.join(path_to_watch, a) 
        print("Added: ", filename)
        if 'jpg' in filename:
            bot.send_picture(filename)
  if removed: 
    print("Removed: ", ", ".join (removed))
  before = after

bot.join()


