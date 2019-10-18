#!/bin/bash

BACKUP_DIR=/home/pi/backup
EXCLUDE_FILE=rsync-exclude.txt

cd $BACKUP_DIR
mkdir backup
sudo rsync -avH --delete-during --delete-excluded --exclude-from=./$EXCLUDE_FILE / $BACKUP_DIR/backup

NUM_FILES_TO_KEEP=2
HOST=`hostname -s`
FILENAME=$BACKUP_DIR/archive_$HOST\_`date +%Y.%m.%d`.tar.gz 

cd backup

logger -s "Starting backup compression"
sudo tar czf $FILENAME ./
logger -s "Finished backup compression"

#restore:
#sudo rsync -avH $BACKUP_DIR/backup /
