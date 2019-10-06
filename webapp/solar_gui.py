#!/usr/bin/python3

from tkinter import *
from tkinter import ttk
from threading import Thread
import time
import signal
from solar_data import SolarData
import json
from nanomsg import Socket, PUB, SUB, SUB_SUBSCRIBE

class SolarTkApp(Thread):
    initialized = False
    def run(self):
        self.tk = Tk()
        self.tk.attributes('-zoomed', True)  # This just maximizes it so we can see the window. It's nothing to do with fullscreen.
        self.tk.attributes('-fullscreen', True)
        self.tk.config(cursor='none')

        self.mainFrame = ttk.Frame(self.tk)
        self.mainFrame.pack()

        #self.titleLabel = ttk.Label(self.tk, text="VanBot")
        #self.titleLabel.config(font=("Courier", 22))
        #self.titleLabel.pack()

        #self.labelClock = ttk.Label(self.tk, text="Time")
        #self.labelClock.config(font=("Courier", 18))
        #self.labelClock.pack()

        self.label = ttk.Label(self.tk, text="<3 Annika <3", justify=LEFT)
        self.label.config(font=("Courier", 12))
        self.label.pack(side = LEFT)
        self.update_clock()
        
        self.initialized = True
        self.tk.mainloop()

    def sigint_handler(self, sig, frame):
        self.tk.quit()
        self.tk.update()

    def update_clock(self):
        now = time.strftime("%H:%M:%S")
        #self.labelClock.configure(text=now)
        self.tk.after(1000, self.update_clock)

    def new_data(self, data):
        if self.initialized:
            self.label.configure(text=data)
        pass

if __name__ == '__main__':
    app = SolarTkApp()
    # Set signal before starting
    signal.signal(signal.SIGINT, app.sigint_handler)

    app.start()
    with Socket(SUB) as s:
        s.connect("tcp://localhost:5555".encode())
        s.set_string_option(SUB, SUB_SUBSCRIBE, ''.encode())
        while True:
            data = s.recv().decode()
            data = data.replace("'", "\"")
            dictData = json.loads(data)
            str_data = ""
            for d in dictData:
                str_data += "{0}: {1}\n".format(d, dictData[d])
            app.new_data(str_data)



