from track_data import TrackParser
from resources import Tools


def main():
    parser = TrackParser(file_path='/home/dexter/scripts/sailcam/PythonImageProcessor/track_data/Mac2020.csv')
    tools = Tools.Tools()
    tools.pretty_print_objects(objects=parser.track_data, objFilter='geo_d')


if __name__ == '__main__':
    main()
