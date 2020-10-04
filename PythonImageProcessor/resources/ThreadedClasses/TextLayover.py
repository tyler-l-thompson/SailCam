from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
from time import time
from resources.ThreadedClasses import ThreadedClass
from track_data import TrackData


class TextLayoverThread(ThreadedClass):

    def __init__(self,
                 file_path,
                 timestamp_text,
                 track_data: 'TrackData',
                 text_color='#AEAEAE',
                 text_location=(2400, 1850),
                 text_font=ImageFont.truetype(font='/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf', size=40)):
        super().__init__()

        self.file_path = file_path
        self.timestamp_text = timestamp_text
        self.track_data = track_data
        print(track_data.timestamp)
        self.text_color = text_color
        self.text_location = text_location
        self.text_font = text_font

    def run(self):
        start_time = time()
        img = Image.open(fp=self.file_path)
        draw = ImageDraw.Draw(im=img)
        draw.text(xy=self.text_location,
                  text=f"{self.timestamp_text}\n{self.track_data.speed} Kts {self.track_data.direction}",
                  fill=self.text_color,
                  font=self.text_font)
        img.save(fp=self.file_path, overwrite=True)
        self.runtime = time() - start_time

    def join(self, **kwargs):
        pass
