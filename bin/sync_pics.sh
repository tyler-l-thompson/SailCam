#!/bin/bash
MOUNTPOINT=/media
mount /dev/sdj1 $MOUNTPOINT
FILECOUNT=$(ls -1 $MOUNTPOINT/*/*/*/ | wc -l)
#rsync -ha --info=progress2 $MOUNTPOINT/ /home/dexter/Pictures/ | pv -lpe -s $FILECOUNT
rsync -aix $MOUNTPOINT/ /home/dexter/Pictures/ | pv -lpes $FILECOUNT >/dev/null
#rm -rf $MOUNTPOINT/*
sync
umount $MOUNTPOINT
