import csv
import pyproj
from datetime import datetime
from pytz import timezone
from resources import Resource


class TrackParser(Resource):

    def __init__(self, file_path: str):
        Resource.__init__(self)
        self.track_data = self.get_track_data(file_path=file_path)
        self.track_data.sort(key=lambda x: x.timestamp, reverse=False)
        self.populate_data()

    def get_track_data(self, file_path: str) -> 'list[TrackData]':
        track_data = []
        with open(file_path, newline='') as csv_file:
            csv_data = csv.DictReader(f=csv_file)
            for row in csv_data:
                track_data.append(TrackData(latitude=row[self.config['track_parser']['indexes']['latitude']],
                                            longitude=row[self.config['track_parser']['indexes']['longitude']],
                                            elevation=row[self.config['track_parser']['indexes']['elevation']],
                                            timestamp=row[self.config['track_parser']['indexes']['timestamp']]))
        return track_data

    def populate_data(self):
        for i in range(1, len(self.track_data)-1):
            self.track_data[i].generate_speed_direction(last_track_data=self.track_data[i-1])

    def get_track_from_timestamp(self, timestamp: datetime) -> 'TrackData':
        if timestamp < self.track_data[0].timestamp:
            return self.track_data[0]

        time_delta = (timestamp - self.track_data[0].timestamp).seconds
        for i in range(1, len(self.track_data) - 1):
            if time_delta <= 60 and \
                    timestamp.year == self.track_data[i].timestamp.year and \
                    timestamp.month == self.track_data[i].timestamp.month and \
                    timestamp.day == self.track_data[i].timestamp.day:
                return self.track_data[i]
            else:
                time_delta = (timestamp - self.track_data[i].timestamp).seconds

        return self.track_data[len(self.track_data) - 1]


class TrackData(object):

    def __init__(self, latitude, longitude, elevation, timestamp):
        self.geo_d = pyproj.Geod(ellps='WGS84')
        self.latitude = (float(latitude))
        self.longitude = (float(longitude))
        self.elevation = float(elevation)
        self.timestamp = datetime.strptime(timestamp, '%Y-%m-%dT%H:%M:%SZ')  # 2020-07-11T12:48:21Z

        # fix timestamp timezone
        self.timestamp = self.timestamp.replace(tzinfo=timezone('GMT'))
        self.timestamp = self.timestamp.astimezone(timezone('US/Eastern'))
        self.timestamp = self.timestamp.replace(tzinfo=None)

        self.speed = 0
        self.direction = 'N'
        self.distance = 0
        self.accumulated_distance = 0
        self.azimuth = 0

    def generate_speed_direction(self, last_track_data: 'TrackData'):
        azimuth1, azimuth2, distance = self.geo_d.inv(lons1=last_track_data.longitude,
                                                      lats1=last_track_data.latitude,
                                                      lons2=self.longitude,
                                                      lats2=self.latitude)
        self.distance = distance
        self.azimuth = azimuth1
        self.direction = self.degree_to_cardinal(
            degrees_f=self.azimuth + 360) if self.azimuth < 0 else self.degree_to_cardinal(degrees_f=self.azimuth)

        self.speed = ((self.distance / (self.timestamp - last_track_data.timestamp).seconds) * 1.9438444924574).__round__(2)
        self.accumulated_distance = (self.distance / 1852) + last_track_data.accumulated_distance

    @staticmethod
    def degree_to_cardinal(degrees_f: float):
        val = int((degrees_f / 22.5) + .5)
        arr = ["N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"]
        return arr[(val % 16)]
