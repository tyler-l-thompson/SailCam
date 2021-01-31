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
    threads = []
    thread_pool = ThreadPool()
    parser = TrackParser(file_path='/home/dexter/scripts/sailcam/PythonImageProcessor/track_data/Mac2020.csv')
    logger.info(f"Processing directory {args.f}")

    clean_directory(dir_path=args.f, backup_path=f"{args.f}_bak") if args.clean else None

    # prepare the threads
    logger.info(msg="Preparing threaded processor...")
    day = f"{args.f[len(args.f) - 2:len(args.f)]}"
    month = f"{args.f[len(args.f) - 5:len(args.f) - 3]}"
    year = f"{args.f[len(args.f) - 10:len(args.f) - 6]}"
    for picture in os.listdir(path=args.f):
        hour = picture[0:2]
        minute = picture[3:5]
        second = picture[6:8]
        timestamp = datetime.strptime(f"{year}-{month}-{day} {hour}:{minute}:{second}", "%Y-%m-%d %H:%M:%S")
        threads.append(TextLayoverThread(file_path=f"{args.f}/{picture}",
                                         timestamp=timestamp,
                                         track_data=parser.get_track_from_timestamp(timestamp=timestamp)))

    thread_pool.run_thread_pool(threads=threads)

    generate_timelapse(dir_path=args.f) if args.timelapse else None
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

    options, args = parser.parse_args()
    return options


def clean_directory(dir_path, backup_path):
    logger.info(msg="Cleaning directory...")
    shutil.rmtree(path=dir_path)
    shutil.copytree(src=backup_path, dst=dir_path)


def generate_timelapse(dir_path: str):
    logger.info(msg='Generating timelapse...')
    subprocess.run([f'cd {dir_path}; /home/dexter/scripts/sailcam/bin/generate_timelapse.sh'], capture_output=True)


if __name__ == '__main__':
    main()
