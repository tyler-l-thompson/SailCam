#!/usr/bin/python3
import optparse
import os
import math
import sys
import subprocess
import shutil
from time import sleep
from datetime import datetime, timedelta
from resources.ThreadedClasses.TextLayover import TextLayoverThread
from resources.ThreadedClasses import ThreadPool
from track_data import TrackParser
from resources.config import config
from resources import logging

logger = logging.get_logger(__name__)


def main():
    start_time: datetime = datetime.now()
    args = get_args()

    logger.info(f"Processing directory {args.f}")

    clean_directory(dir_path=args.f, backup_path=f"{args.f}_bak") if args.clean else None

    annotate_images(dir_path=args.f, track_data_path=args.track_data) if args.annotate else None

    generate_timelapse(dir_path=args.f, fps=config['timelapse']['fps']) if args.timelapse else None

    runtime: timedelta = (datetime.now() - start_time)
    logger.info(msg=f'Done. Runtime: {math.floor(runtime.total_seconds()/60)} Minutes {runtime.seconds} Seconds')


def get_args():
    """
    Get command line arguments.
    :return: optparse.options
    """
    parser = optparse.OptionParser(usage='%prog [options]',
                                   description=""
                                   )
    parser.add_option("-v",
                      "--verbose",
                      default=False,
                      action="store_true"
                      )
    parser.add_option('-f',
                      action="store",
                      default="/home/dexter/Pictures/2020/07/11",
                      help="Path to the picture directory.")
    parser.add_option('-c',
                      '--clean',
                      default=False,
                      action='store_true')
    parser.add_option('-l',
                      '--timelapse',
                      default=False,
                      action='store_true')
    parser.add_option('-a',
                      '--annotate',
                      default=False,
                      action='store_true')
    parser.add_option('-d',
                      '--track_data',
                      action="store",
                      default="/home/dexter/scripts/sailcam/PythonImageProcessor/track_data/Mac2022.csv",
                      help="Path to track data.")

    options, args = parser.parse_args()
    return options


def annotate_images(dir_path: str, track_data_path: str):
    threads = []
    thread_pool = ThreadPool()
    parser = TrackParser(file_path=track_data_path)

    # prepare the threads
    logger.info(msg="Preparing threaded processor...")
    day = f"{dir_path[len(dir_path) - 2:len(dir_path)]}"
    month = f"{dir_path[len(dir_path) - 5:len(dir_path) - 3]}"
    year = f"{dir_path[len(dir_path) - 10:len(dir_path) - 6]}"
    for picture in os.listdir(path=dir_path):
        if ".avi" in picture:
            continue
        hour = picture[9:11]
        minute = picture[11:13]
        second = picture[13:15]
        timestamp = datetime.strptime(f"{year}-{month}-{day} {hour}:{minute}:{second}", "%Y-%m-%d %H:%M:%S")
        threads.append(TextLayoverThread(file_path=f"{dir_path}/{picture}",
                                         timestamp=timestamp,
                                         track_data=parser.get_track_from_timestamp(timestamp=timestamp)))

    thread_pool.run_thread_pool(threads=threads)


def clean_directory(dir_path, backup_path):
    logger.info(msg="Cleaning directory...")
    shutil.rmtree(path=dir_path)
    shutil.copytree(src=backup_path, dst=dir_path)


def generate_timelapse(dir_path: str, fps: str):
    logger.info(msg='Generating timelapse...')
    subprocess.Popen(['/home/dexter/scripts/sailcam/bin/generate_timelapse.sh', dir_path, fps])


if __name__ == '__main__':
    main()
