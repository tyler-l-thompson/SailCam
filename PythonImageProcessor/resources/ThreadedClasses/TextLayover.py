import cv2
from PIL import Image, ImageFont, ImageDraw
from time import time
from datetime import datetime, timedelta
from resources.ThreadedClasses import ThreadedClass
from track_data import TrackData
# import matplotlib.pyplot as plt
# import numpy as np


class TextLayoverThread(ThreadedClass):

    def __init__(self,
                 file_path: str,
                 timestamp: datetime,
                 track_data: 'TrackData' = None,
                 text_color: str = '#AEAEAE',
                 text_font: ImageFont.FreeTypeFont = ImageFont.truetype(font='/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf', size=40),
                 start_of_race_timestamp: datetime = datetime.strptime('2022-07-16 11:40:00', '%Y-%m-%d %H:%M:%S'),
                 dock_leave_timestamp: datetime = datetime.strptime('2022-07-16 09:40:00', '%Y-%m-%d %H:%M:%S'),
                 end_of_race_timestamp: datetime = datetime.strptime('2022-07-18 10:00:00', '%Y-%m-%d %H:%M:%S')):
        super().__init__()

        self.file_path = file_path
        self.timestamp = timestamp
        self.track_data = track_data if track_data else TrackData(0, 0, 0, timestamp.strftime('%Y-%m-%dT%H:%M:%SZ'))
        self.text_color = text_color
        self.text_font = text_font
        self.dock_leave_timestamp = dock_leave_timestamp
        self.start_of_race_timestamp = start_of_race_timestamp
        self.end_of_race_timestamp = end_of_race_timestamp

    def run(self):
        image_x = 2592
        image_y = 1944
        crop_top = 0
        crop_bottom = 300
        crop_left = 425
        crop_right = 525

        start_time = time()
        # print(f"{self.timestamp.strftime('%Y %M %D %H %m %S')} | {self.track_data.timestamp.strftime('%Y %M %D %H %m %S')}")
        if self.timestamp >= self.dock_leave_timestamp:
            time_on_water = self.timestamp - self.dock_leave_timestamp
        else:
            time_on_water = timedelta(0)

        # opencv processing
        # if self.timestamp.hour > 21 or self.timestamp.hour < 6:
        try:
            image = cv2.imread(self.file_path)
            image, alpha, beta = self.automatic_brightness_and_contrast(image)
            cv2.imwrite(self.file_path, image)
        except Exception as e:
            pass
            self.logger.error(f'Error while correcting image: {e}')

        try:
            # text layover processing
            img = Image.open(fp=self.file_path)
            draw = ImageDraw.Draw(im=img)

            hours = int(time_on_water.total_seconds() / 3600)
            minutes = int((time_on_water.total_seconds() - (hours * 3600)) / 60)

            # image size: 2592x1944
            # text size: 40
            # line height: 35
            # character width: 25

            character_width = 25
            character_count = 13

            line_height = 35
            line_count = 5

            left_justification = image_x - (character_width * character_count) - crop_right
            top_justification = image_y - (line_height * line_count) - crop_bottom
            shape = [(left_justification, top_justification), (image_x - crop_left, image_y - crop_bottom)]
            draw.rectangle(shape, fill=(1, 0, 0))

            draw.text(xy=(left_justification, top_justification),
                      text=f"TOD: {self.timestamp.strftime('%I:%M %p')}\n"
                           f" ET: {hours:02}:{minutes:02}\n"
                           f"TRK: {self.track_data.accumulated_distance.__round__(1):05} NM\n"
                           f"SOG: {self.track_data.speed:05.2f} KN\n"
                           f"BOD: {self.track_data.direction}",
                      fill=self.text_color,
                      font=self.text_font)

            img.save(fp=self.file_path, overwrite=True)
        except Exception as e:
            self.logger.error(f'Error while adding annotation to image: {e}')

        # crop
        try:
            orig_img = cv2.imread(self.file_path)
            crop_img = orig_img[crop_top:image_y - crop_bottom, crop_left:image_x - crop_right]
            cv2.imwrite(self.file_path, crop_img)
        except Exception as e:
            self.logger.error(f'Error while cropping image: {e}')

        self.runtime = time() - start_time

    def join(self, **kwargs):
        pass

    # Automatic brightness and contrast optimization with optional histogram clipping
    @staticmethod
    def automatic_brightness_and_contrast(image: Image, clip_hist_percent: float = 2.5) -> 'auto_result, alpha, beta':
        gray = cv2.cvtColor(image, cv2.COLOR_RGB2HLS_FULL)

        # Calculate grayscale histogram
        hist = cv2.calcHist([gray], [0], None, [256], [0, 256])
        hist_size = len(hist)

        # Calculate cumulative distribution from the histogram
        accumulator = []
        accumulator.append(float(hist[0]))
        for index in range(1, hist_size):
            accumulator.append(accumulator[index - 1] + float(hist[index]))

        # Locate points to clip
        maximum = accumulator[-1]
        clip_hist_percent *= (maximum/100.0)
        clip_hist_percent /= 2.0

        # Locate left cut
        minimum_gray = 0
        while accumulator[minimum_gray] < clip_hist_percent:
            minimum_gray += 1

        # Locate right cut
        maximum_gray = hist_size - 1
        while accumulator[maximum_gray] >= (maximum - clip_hist_percent):
            maximum_gray -= 1

        # Calculate alpha and beta values
        alpha = 255 / (maximum_gray - minimum_gray)
        beta = -minimum_gray * alpha

        # Calculate new histogram with desired range and show histogram 
        # new_hist = cv2.calcHist([gray], [0], None, [256], [minimum_gray, maximum_gray])
        # plt.plot(hist)
        # plt.plot(new_hist)
        # plt.xlim([0, 256])
        # plt.xticks(np.arange(0, 255, 10))
        # plt.show()
        # print(alpha, beta, maximum_gray, minimum_gray)
        # print(gray)

        auto_result = cv2.convertScaleAbs(image, alpha=alpha, beta=beta)
        return auto_result, alpha, beta
