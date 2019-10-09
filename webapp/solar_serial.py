from threading import Thread
from solar_data import SolarData
import serial
from time import sleep
from pynng import Pub0, Sub0, Pair1, TryAgain

class SolarSerial(Thread):
    def __init__(self, settings):
        super().__init__()
        self.running = False
        self.settings = settings
        self.serial = None
        self.connected = False
        self.pubSocket = Pub0()
        self.msgSocket = Pair1(polyamorous=True)

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
            print(data)
            self.pubSocket.send(sd.to_byte())
        else:
            print("Cannot parse data: ", data)
        pass

    def run(self):
        self.running = True
        self.pubSocket.listen(self.settings['address_pub'])
        self.msgSocket.listen(self.settings['address_msg'])

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
                try:
                    send_data = self.msgSocket.recv(block=False)
                    self.serial.write(send_data)
                except TryAgain:
                    #no data
                    pass

            sleep(5)

if __name__ == "__main__":
    from solar_settings import SolarSettings
    ss = SolarSerial(SolarSettings.serial_settings)
    ss.start()
    ss.join()
