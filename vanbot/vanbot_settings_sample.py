class VanBotSettings:
    #vanbot token
    token = ''
    #vanbot master user
    username = ''
    #vanbot chatid with masteruser
    chat_id = 12345
    webcam = {
                'inside':  {
                    'alarm_time_seconds': 5,
                    'min_alarm_duration': 5,
                    'min_motion_area': 300,
                    'photo_filename': '/tmp/vanbot_photo_inside',
                    'video_filename': '/tmp/vanbot_rec_inside',
                    'rec_resolution': (640, 480),
                    'rec_framerate': 20.0,
                    'webcam_source': 0,
                    'resolution': (640, 480),
                    'framerate': 20.0
                }
            }
    upload_settings = {
        'hostname': 'localhost',
        'username': 'vanbot',
        'private_key': 'id_rsa',
        'remote_directory': '/remote/dir/'

    }
    http_streamer = {
        'host': '0.0.0.0',
        'port': 8000,
        'template_folder': '/home/beni/solar/vanbot/templates'
    }

    solar_system = {
        'address_pub': 'ws://127.0.0.1:5555',
        'address_msg': 'ws://127.0.0.1:5556'
    }