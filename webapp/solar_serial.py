from threading import Thread
from queue import Queue
from solar_data import SolarData
import serial
from time import sleep

class SolarSerial(Thread):
    def __init__(self, settings):
        super().__init__()
        self.running = False
        self.settings = settings
        self.serial = None
        self.connected = False
        self.recvQueue = Queue()
        self.sendQueue = Queue()

    def openSerial(self):
        if self.serial is not None:
            self.serial.close()
            self.serial = None
        try:
            self.serial = serial.Serial(self.settings['port'], self.settings['baud'], timeout=0)
            self.connected = True
        except:
            print("Error opening serial port:", self.settings['port'])
            self.connected = False
        
    def closeSerial(self):
        if self.serial is not None:
            self.serial.close()
            self.serial = None

    def stop(self):
        self.running = False

    def handle_data(self, data):
        sd = SolarData(data)
        if sd.initalized:
            self.recvQueue.put(sd)
        else:
            print("Cannot parse data: ", data)
        pass

    def run(self):
        self.running = True
        while self.running:
            self.openSerial()
            data = ""
            while self.connected:          
                try:
                    received_data = self.serial.readline().decode()
                except:
                    #print("Failed to read data")
                    received_data = None
                if received_data:
                    data += received_data
                    if '\r\n' in data:
                        sliced = data.split('\r\n')
                        data = ''.join(sliced[1:])
                        self.handle_data(sliced[0])

                while not self.sendQueue.empty():
                    send_data = self.sendQueue.get()
                    self.serial.write(send_data)
            sleep(5)
                    

if __name__ == "__main__":
    from solar_settings import SolarSettings
    ss = SolarSerial(SolarSettings.serial_settings)
    ss.start()
    ss.join()
