#!/bin/bash
if [ -z "$1" || -z "$2" ]
then
  echo "USAGE: generate_timelapse.sh <IMAGE_DIR> <FPS>"
  exit 0
fi

cd $1

FPS=$2
root="$(pwd)"
IFS='/' read -a array <<< "${root}"
year="${array[6]}"
month="${array[7]}"
day="${array[8]}"

#FILECOUNT=$(ls -1 $root | wc -l)
# echo "Annotating and filtering images..."
#if [ ! -d ./processed ]
#then
#	mkdir -p ./processed
##	for file in *.JPG
##	do
##	    echo "Processing: $file"
##	    hour="${file:0:2}"
##	    minute="${file:3:2}"
##	    second="${file:6:2}"
##
##	    convert $file -auto-level -gravity SouthEast -pointsize 30 -fill "#AEAEAE" -annotate 0 "$month/$day/$year $hour:$minute:$second" processed/$file &
##	    sleep 0.2
##	done | pv -lpes $FILECOUNT > /dev/null
##	sleep 10
#fi
# echo "Done."

echo "Creating timelapse..."
rm -rf ./$year-$month-$day.avi
mencoder mf://*.jpg -mf w=1644:h=1642:fps=$FPS:type=jpg -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=32000000:keyint=15:mbd=2:trell:threads=16 -oac copy -o "$year-$month-$day.avi"

#ffmpeg -r $FPS -pattern_type glob -i '*.JPG' -s 1944x2592 -vcodec libx264 -crf 18 -preset slow "$year-$month-$day.avi"

echo "Done."
