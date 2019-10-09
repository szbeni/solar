#!/usr/bin/python3

from tkinter import *
from tkinter import ttk
from threading import Thread
import time
import signal
from solar_data import SolarData
from solar_settings import SolarSettings
import json
import os
from pynng import Sub0, Pair1

class SolarTkApp(Thread):
    initialized = False
    running = True
    def run(self):
        self.tk = Tk()
        self.tk.attributes('-zoomed', True)  # This just maximizes it so we can see the window. It's nothing to do with fullscreen.
        self.tk.attributes('-fullscreen', True)
        self.tk.config(cursor='none')

        self.mainFrame = ttk.Frame(self.tk)
        self.mainFrame.pack()

        self.titleLabel = ttk.Label(self.tk, text="VanBot")
        self.titleLabel.config(font=("Courier", 22))
        self.titleLabel.pack()

        self.labelClock = ttk.Label(self.tk, text="Time")
        self.labelClock.config(font=("Courier", 18))
        self.labelClock.pack()
        self.update_clock()

        self.label = ttk.Label(self.tk, text="<3 Annika <3")
        self.label.config(font=("Courier", 12))
        self.label.pack()

        self.buttonShutdown = ttk.Button(self.tk, text="ShutDown", command=self.command_shutdown)
        self.buttonShutdown.pack()

        self.initialized = True
        self.running = True
        self.tk.mainloop()

    def sigint_handler(self, sig, frame):
        self.tk.quit()
        self.tk.update()
        self.running = False

    def update_clock(self):
        now = time.strftime("%H:%M:%S")
        self.labelClock.configure(text=now)
        self.tk.after(1000, self.update_clock)

    def new_data(self, data):
        if self.initialized:
            self.label.configure(text=data)
        pass

    def command_shutdown(self):
        print("shutdown")
        os.system("sudo init 0") 

if __name__ == '__main__':
    app = SolarTkApp()
    # Set signal before starting
    signal.signal(signal.SIGINT, app.sigint_handler)
    app.start()
    subSocket = Sub0(dial=SolarSettings.serial_pub_address)
    subSocket.subscribe(b'solardata')
    msgSocket = Pair1(dial=SolarSettings.serial_msg_address)

    while app.running:
        data = subSocket.recv()
        sd = SolarData(from_byte=data)
        dictData = sd.as_dict()
        str_data = ""
        i = 1
        for d in dictData:
            if d == 'time':
                continue
            if (i % 2) == 1:
                if i != 1:
                    str_data += '\n'
                str_data += "{0: <5}: ".format(d)
            else:
                str_data += "\t{0: <5}: ".format(d)

            if str(dictData[d]).isdigit():
                str_data += "{0: <5}".format(dictData[d])
            else:
                str_data += "{0: <5,.3f}".format(dictData[d])
            i += 1
        app.new_data(str_data)