class SolarSettings:
    fps =  100.0
    serial_pub_address = "ws://127.0.0.1:5555"
    serial_msg_address = "ws://127.0.0.1:5556"
    serial_settings = {
        'address_pub': 'ws://127.0.0.1:5555',
        'address_msg': 'ws://127.0.0.1:5556',
        'port': '/dev/rfcomm0',
        'baud': 9600,
        'no_data_restart_time': 5
    }
