#!/bin/bash
FPS=20
root="$(pwd)"
IFS='/' read -a array <<< "${root}"
year="${array[4]}"
month="${array[5]}"
day="${array[6]}"
FILECOUNT=$(ls -1 $root | wc -l)
echo "Annotating and filtering images..."
if [ ! -d ./processed ]
then
	mkdir -p ./processed
	for file in *.JPG
	do
	    echo "Processing: $file"
	    hour="${file:0:2}"
	    minute="${file:3:2}"
	    second="${file:6:2}"

<<<<<<< HEAD
	    convert $file -auto-level -gravity SouthEast -pointsize 30 -fill "#AEAEAE" -annotate 0 "$month/$day/$year $hour:$minute:$second" processed/$file &
	    sleep 0.2
	done | pv -lpes $FILECOUNT > /dev/null
	sleep 10
fi
echo "Done."
echo "Creating timelapse..."
rm -rf ./processed/$year-$month-$day.avi
mencoder mf://processed/*.JPG -mf w=1944:h=2592:fps=$FPS:type=jpg -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=24000000:keyint=15:mbd=2:trell -oac copy -o "$year-$month-$day.avi"
echo "Done."
=======
mkdir ./processed
for file in *.jpg
do
    echo "Processing: $file"
    hour="${file:0:2}"
    minute="${file:3:2}"
    second="${file:6:2}"

    convert $file -gravity SouthEast -pointsize 30 -fill "#AEAEAE" -annotate 0 "$month/$day/$year $hour:$minute:$second" processed/$file &
done
sleep 10
mencoder mf://processed/*.jpg -mf w=1944:h=2592:fps=6:type=jpg -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=24000000:keyint=15:mbd=2:trell -oac copy -o "$year-$month-$day.avi"
>>>>>>> 03048015de9f53dc74c612c6c089b3cb3c84ef13
