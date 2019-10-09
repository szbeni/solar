from solar_settings import SolarSettings
from solar_data import SolarData
from pynng import Pair1, Sub0, TryAgain

#serial data in and out
subSocket = Sub0(dial=SolarSettings.serial_pub_address)
subSocket.subscribe(b'')
msgSocket = Pair1(dial=SolarSettings.serial_msg_address, polyamorous=True)

while True:
    data = subSocket.recv(block)
    sd = SolarData(from_byte=data)
    print(sd)
