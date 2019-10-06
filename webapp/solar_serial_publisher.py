from threading import Thread
from time import sleep
from nanomsg import Socket, PUB, SUB, SUB_SUBSCRIBE

class SolarSerialPublisher(Thread):
    def __init__(self, settings):
        super().__init__()
        self.running = False
        self.settings = settings
        self.s = Socket(PUB)

    def stop(self):
        self.running = False

    def run(self):
        self.s.bind(self.settings['socket_address'].encode())

        self.running = True
        while self.running:
#            self.s.send(b'bsd456')
            sleep(0.1)
            pass

    def new_data(self, data):
        self.s.send(data)

if __name__ == "__main__":
#    from solar_settings import SolarSettings
#    ssp = SolarSerialPublisher(SolarSettings.serial_pub_settings)
#    ssp.start()

    with Socket(SUB) as s:
        s.connect("tcp://localhost:5555".encode())
        s.set_string_option(SUB, SUB_SUBSCRIBE, ''.encode())
        while True:
            data = s.recv()
            print("New data:")
            print(data)
#    ssp.join()


