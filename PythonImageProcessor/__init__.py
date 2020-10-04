import optparse
import os
import sys
from time import sleep
from datetime import datetime
from resources.ThreadedClasses.TextLayover import TextLayoverThread
from resources.ThreadedClasses import ThreadPool
from track_data import TrackParser
from resources.config import config
from resources import logging

logger = logging.get_logger(__name__)


def main():
    args = get_args()
    threads = []
    thread_pool = ThreadPool()
    parser = TrackParser(file_path='/home/dexter/scripts/sailcam/PythonImageProcessor/track_data/Mac2020.csv')
    logger.info(f"Processing directory {args.f}")

    # prepare the threads
    day = f"{args.f[len(args.f) - 2:len(args.f)]}"
    month = f"{args.f[len(args.f) - 5:len(args.f) - 3]}"
    year = f"{args.f[len(args.f) - 10:len(args.f) - 6]}"
    for picture in os.listdir(path=args.f):
        hour = picture[0:2]
        minute = picture[3:5]
        second = picture[6:8]
        threads.append(TextLayoverThread(file_path=f"{args.f}/{picture}",
                                         track_data=parser.get_track_from_timestamp(timestamp=datetime.strptime(f"{year}-{month}-{day} {picture[0:2]}:{picture[3:5]}:{picture[6:8]}", "%Y-%m-%d %H:%M:%S")),
                                         timestamp_text=f"{hour}:{minute}:{second}"))

    thread_pool.run_thread_pool(threads=threads)


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

    options, args = parser.parse_args()
    return options


if __name__ == '__main__':
    main()
