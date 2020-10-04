import csv
import pyproj
from datetime import datetime
from resources import Resource


class TrackParser(Resource):

    def __init__(self, file_path):
        Resource.__init__(self)
        self.track_data = self.get_track_data(file_path=file_path)
        self.populate_data()

    def get_track_data(self, file_path):
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
        time_delta = (timestamp - self.track_data[0].timestamp).seconds
        if time_delta <= 0:
            return self.track_data[0]

        for i in range(1, len(self.track_data) - 1):
            if time_delta <= 0:
                return self.track_data[i-1]
            else:
                time_delta = (timestamp - self.track_data[i].timestamp).seconds

        return self.track_data[len(self.track_data) - 1]


class TrackData(object):

    def __init__(self, latitude, longitude, elevation, timestamp):
        self.geo_d = pyproj.Geod(ellps='WGS84')
        self.latitude = latitude
        self.longitude = longitude
        self.elevation = elevation
        self.timestamp = datetime.strptime(timestamp, '%Y-%m-%dT%H:%M:%SZ')  # 2020-07-11T12:48:21Z
        self.speed = 0
        self.direction = 'N'
        self.azimuth = 0

    def generate_speed_direction(self, last_track_data: 'TrackData'):
        azimuth1, azimuth2, distance = self.geo_d.inv(lons1=last_track_data.longitude,
                                                      lats1=last_track_data.latitude,
                                                      lons2=self.longitude,
                                                      lats2=self.latitude)
        self.speed = ((distance / (self.timestamp - last_track_data.timestamp).seconds) / 0.514444444).__round__(2)
        self.azimuth = azimuth1
        self.direction = self.degree_to_cardinal(degrees=self.azimuth + 360) if self.azimuth < 0 else self.degree_to_cardinal(degrees=self.azimuth)

    @staticmethod
    def degree_to_cardinal(degrees):
        val = int((degrees / 22.5) + .5)
        arr = ["N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"]
        return arr[(val % 16)]
