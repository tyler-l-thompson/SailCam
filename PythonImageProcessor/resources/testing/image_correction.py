from datetime import datetime
import time
from resources.ThreadedClasses.TextLayover import TextLayoverThread


def main():
    sample_thread = TextLayoverThread(file_path="/home/dexter/Pictures/mac2022photos/images/2022/07/16/20220716_092915.jpg",
                                      timestamp=datetime.strptime("2022-07-16 09:29:15", "%Y-%m-%d %H:%M:%S"))
    sample_thread.run()
    while sample_thread.is_alive():
        time.sleep(0.1)
    sample_thread.join()
    print("Done.")


if __name__ == '__main__':
    main()
