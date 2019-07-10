#!/bin/bash
mount /dev/sdg1 /media
rsync -hav /media/ /home/dexter/Pictures/
#rm -rf /media/*
sync
umount /media
