#!/bin/bash
root="$(pwd)"
IFS='/' read -a array <<< "${root}"
year="${array[4]}"
month="${array[5]}"
day="${array[6]}"

mkdir ./rotated
for file in *.JPG
do
    echo "Rotating: $file"
    hour="${file:0:2}"
    minute="${file:3:2}"
    second="${file:6:2}"

    convert $file -rotate 90 -gravity SouthEast -pointsize 30 -annotate 0 "$month/$day/$year $hour:$minute:$second" rotated/$file &
done
sleep 10
mencoder mf://rotated/*.JPG -mf w=1944:h=2592:fps=5:type=jpg -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=24000000:keyint=15:mbd=2:trell -oac copy -o "$year-$month-$day.avi"
