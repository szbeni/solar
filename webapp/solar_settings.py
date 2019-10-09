class SolarSettings:
    fps =  100.0
    serial_pub_address = "tcp://localhost:5555"
    serial_msg_address = "tcp://localhost:5556"
    serial_settings = {
        'address_pub': 'tcp://0.0.0.0:5555',
        'address_msg': 'tcp://0.0.0.0:5556',
        'port': '/dev/rfcomm0',
        'baud': 9600
    }
