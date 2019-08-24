from threading import Thread
from queue import Queue
from solar_data import SolarData
import serial

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
            while self.connected:            
                data = self.serial.readline().decode()
                if data:
                    self.handle_data(data)

                while not self.sendQueue.empty():
                    data = self.sendQueue.get()
                    self.serial.write(data)
                    

if __name__ == "__main__":
    from solar_settings import SolarSettings
    ss = SolarSerial(SolarSettings.serial_settings)
    ss.start()
    ss.join()
