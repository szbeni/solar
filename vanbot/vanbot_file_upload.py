import os
import pysftp
from time import sleep
from threading import Thread
from vanbot_settings import VanBotSettings


class VanBotFileUpload(Thread):

    def __init__(self, settings):
        super().__init__()  
        self.hostname = settings['hostname']
        self.username = settings['username']
        self.private_key = settings['private_key']
        self.remoteDir = settings['remote_directory']
        self.uploadFileList = []
        self.running = False

    def upload(self, filename):
        self.uploadFileList.append(filename)

    def stop(self):
        self.running = False

    def run(self):
        self.running = True
        while self.running:
            if len(self.uploadFileList):
                filename = self.uploadFileList.pop()
                self._upload(filename)
            sleep(0.1)

    def _upload(self, filename):

        with pysftp.Connection(host=self.hostname, username=self.username, private_key=self.private_key) as sftp:
            print("Connection succesfully stablished ... ")

            localFilePath = filename
            # Define the remote path where the file will be uploaded
            remoteFilePath = os.path.join(self.remoteDir, os.path.basename(localFilePath))
            print("Upload {0} to {1}".format(localFilePath, remoteFilePath))
            sftp.put(localFilePath, remoteFilePath)
        print("Upload Successful")

if __name__ == "__main__":
    fu = VanBotFileUpload(VanBotSettings.upload_settings)
    fu.start()
    fu.upload('/home/beni/asd.txt')
    sleep(10)
    fu.stop()
    fu.join()
