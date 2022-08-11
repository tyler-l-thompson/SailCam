from datetime import datetime
import time
from resources.ThreadedClasses.TextLayover import TextLayoverThread
from track_data import TrackParser
from resources import Tools


def main():
    timestamp = datetime.strptime("2022-07-16 09:29:15", "%Y-%m-%d %H:%M:%S")
    parser = TrackParser(file_path="/home/dexter/scripts/sailcam/PythonImageProcessor/track_data/Mac2022.csv")

    track_data = parser.get_track_from_timestamp(timestamp=timestamp)
    tools = Tools.Tools()
    tools.pretty_print_objects([track_data])


    # sample_thread = TextLayoverThread(file_path="/home/dexter/Pictures/mac2022photos/images/2022/07/16/20220716_092915.jpg",
    #                                   timestamp=timestamp,
    #                                   track_data=parser.get_track_from_timestamp(timestamp=timestamp))
    # sample_thread.run()
    # while sample_thread.is_alive():
    #     time.sleep(0.1)
    # sample_thread.join()
    print("Done.")


if __name__ == '__main__':
    main()
