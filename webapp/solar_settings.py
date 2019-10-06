class SolarSettings:
    fps =  100.0
    serial_settings = {
        'port': '/dev/rfcomm0',
        'baud': 9600
    }
    serial_pub_settings = {
        'socket_address': 'tcp://*:5555'
    }